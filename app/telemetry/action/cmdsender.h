#ifndef CMDSENDER_H
#define CMDSENDER_H

#include "../util/mavlink_include.h"

#include <optional>
#include <functional>
#include <mutex>
#include <list>
#include <thread>
#include <memory>

/**
 * Simple, threadsafe class for sending mavlink command (long) over a lossy link.
 */
class CmdSender
{
public:
    CmdSender();
    static CmdSender& instance();
    /**
     * returns true if this message has been consumed, false otherwise.
     */
    bool process_message(const mavlink_message_t& msg);

    struct RunCommandResult{
        // Response from the recipient, if there is any (otherwise, the message got lost on each re-transmit)
        std::optional<mavlink_command_ack_t> opt_ack;
        // How often this command was transmitted until success / failure
        int n_transmissions=-1;
    };
    typedef std::function<void(RunCommandResult result)> RESULT_CB;
    /**
     * enqueues a commmand to be sent (with retransmissions). If the queue has reached its limit, false is returned and the result cb is not called.
     * Otherwise, the result cb is quaranteed to be called after either
     * an ack (success/denied) from the recipient is received or the command timed out (no response from the recipient)
     */
    bool send_command_long_async(const mavlink_command_long_t cmd,RESULT_CB result,std::chrono::milliseconds retransmit_delay=std::chrono::milliseconds(500),int n_wanted_retransmissions=3);

    enum Result{
        QUEUE_FULL, // cannot enqueue command, max size reached
        NO_RESPONSE, // no response from recipient
        CMD_DENIED, // response from recipient, but negative (some error code)
        CMD_SUCCESS // positive response from recipient
    };
    /**
     * This blocks the calling thread for up to X second (n retransmissions * timeout)
     * But usefully in some cases.
     * DO NOT CALL THIS FROM THE TELEMETRY RECEIVE THREAD !
     */
    Result send_command_long_blocking(const mavlink_command_long_t cmd,std::chrono::milliseconds retransmit_delay=std::chrono::milliseconds(500),int n_wanted_retransmissions=3);
private:
    struct RunningCommand{
        mavlink_command_long_t cmd;
        RESULT_CB cb;
        // How often this command should be retransmitted
        int n_wanted_retransmissions;
        // Delay between each retransmission
        std::chrono::milliseconds retransmit_delay;
        // How often this command has already been sent
        int n_transmissions=0;
        // last time it was sent
        std::chrono::steady_clock::time_point last_transmission=std::chrono::steady_clock::now();
    };
    bool handle_cmd_ack(const mavlink_command_ack_t& ack);
private:
    std::mutex m_mutex;
    std::list<RunningCommand> m_running_commands;
    // searches for a command refering to this command id
    // if found, remove the command and return it.
    // ootherwise, return nullopt
    std::optional<RunningCommand> find_remove_running_command(int command_id);
    void send_command(RunningCommand& cmd);
    mavlink_message_t pack_command_msg(const mavlink_command_long_t& cmd);

    std::unique_ptr<std::thread> m_timeout_thread;
    void loop_timeout();
    void handle_timeout();
    static constexpr auto MAX_N_SIMULTANOEUS_COMMANDS=5;
    static std::string run_command_result_as_string(const RunCommandResult& res);
};

#endif // CMDSENDER_H
