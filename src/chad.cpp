#include <libssh/libssh.h>
#include <libssh/server.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

std::vector<ssh_session> sessions;
std::mutex session_mutex;

ssh_session session;

void handle_client() {
    ssh_channel channel = ssh_channel_new(session);
  if (channel == nullptr) {
        std::cerr << "Error creating channel: " << ssh_get_error(session) << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        return;
    }

    if (ssh_channel_open_session(channel) != SSH_OK) {
        std::cerr << "Error opening channel session: " << ssh_get_error(session) << std::endl;
        ssh_channel_free(channel);
        ssh_disconnect(session);
        ssh_free(session);
        return;
    }

    if (ssh_channel_request_shell(channel) != SSH_OK) {
        std::cerr << "Error requesting shell: " << ssh_get_error(session) << std::endl;
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        ssh_disconnect(session);
        ssh_free(session);
        return;
    }

    while (true) {
        char buffer[256];
        int nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
        if (nbytes <= 0) {
            break;
        }

        buffer[nbytes] = '\0';
        std::lock_guard<std::mutex> lock(session_mutex);

        for (auto& sess : sessions) {
            if (sess != session) {
                ssh_channel remote_channel = ssh_channel_new(sess);
                if (remote_channel == nullptr) {
                    continue;
                }
                if (ssh_channel_open_session(remote_channel) == SSH_OK) {
                    ssh_channel_write(remote_channel, buffer, nbytes);
                    ssh_channel_close(remote_channel);
                }
                ssh_channel_free(remote_channel);
            }
        }

        ssh_channel_write(channel, buffer, nbytes);
    }

    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(session);
    ssh_free(session);

    std::lock_guard<std::mutex> lock(session_mutex);
    auto it = std::remove(sessions.begin(), sessions.end(), session);
    sessions.erase(it, sessions.end());
}

int main() {
    ssh_bind sshbind = ssh_bind_new();
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, "ssh_host_rsa_key");
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT_STR, "2222");  // Set non-privileged port

    if (ssh_bind_listen(sshbind) != SSH_OK) {
        std::cerr << "Error listening to socket: " << ssh_get_error(sshbind) << std::endl;
        ssh_bind_free(sshbind);
        return -1;
    }

    std::cout << "Server listening on port 2222..." << std::endl;

    while (true) {
        session = ssh_new();
        if (ssh_bind_accept(sshbind, session) == SSH_OK) {
            std::cout << "New session accepted" << std::endl;

            if (ssh_handle_key_exchange(session) != SSH_OK) {
                std::cerr << "Error during key exchange: " << ssh_get_error(session) << std::endl;
                ssh_disconnect(session);
                ssh_free(session);
                continue;
            }

            std::lock_guard<std::mutex> lock(session_mutex);
            sessions.push_back(session);
            std::thread(handle_client).detach();
        } else {
            std::cerr << "Error accepting session: " << ssh_get_error(sshbind) << std::endl;
            ssh_free(session);
        }
    }

    ssh_bind_free(sshbind);
    return 0;
}
