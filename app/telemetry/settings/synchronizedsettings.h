#ifndef SynchronizedSettings_H
#define SynchronizedSettings_H

#include <QObject>
#include <qsettings.h>
#include "../../../lib/lqtutils_master/lqtutils_prop.h"
#include "param_names.h"
#include "../requestmessagehelper.h"

// Helper for settings that MUST STAY IN SYNC on the ground and air pi, since otherwise the wifibroadcast link is lost
// and the user needs to manually recover the link
// It does not fix the 2 general's problem (this is a unfixable problem) but it makes it really unlikely to happen.
// TODO: I think for these 3 (only) parameters that need to be kept in sync in evo, we should find a better way
// (e.g. exposing the param only on the ground, and having a more advanced wifibroadcast imlementation that establishes a connection/
// handles frequency changes itself)
class SynchronizedSettings : public QObject
{
    Q_OBJECT
public:
    explicit SynchronizedSettings(QObject *parent = nullptr);

    static SynchronizedSettings& instance();

    // FLOW: Invalid until first message announcing channel frequency and width is received from the ground
    // When we receive this type of message from the ground, we start requesting the supported frequencies from the ground unit
    // (until successfully requested)
    // Then we know the current frequency, channel width and which channels the ground is capable of -
    // and are ready to populate the choices for the user.
    // Whenever we want to change one of those params, we first try changing it on the air unit
    // (Air unit will reject if it is not capable of the given frequency)
    // and on success, we change the ground unit frequency. Since one can only select frequencies the ground unit supports,
    // this should nevver fail.


    // These are also in aohdsystem, their usage (and correct setting of them) is required here, too
    L_RO_PROP(int,curr_channel_mhz,set_curr_channel_mhz,-1)
    L_RO_PROP(int,curr_channel_width_mhz,set_curr_channel_width_mhz,-1);

    // Set to true once the channels from the ground have been succesfully fetched
    L_RO_PROP(bool,has_fetched_channels,set_has_fetched_channels,false);
    // Dirty
    L_RO_PROP(int,progress_scan_channels_perc,set_progress_scan_channels_perc,-1);
    L_RO_PROP(int,progress_analyze_channels_perc,set_progress_analyze_channels_perc,-1);
    L_RO_PROP(QString,text_for_qml,set_text_for_qml,"NONE");
public:
    void process_message_openhd_wifibroadcast_supported_channels(const mavlink_openhd_wifbroadcast_supported_channels_t& msg);
    void process_message_openhd_wifibroadcast_analyze_channels_progress(const mavlink_openhd_wifbroadcast_analyze_channels_progress_t& msg);
    void process_message_openhd_wifibroadcast_scan_channels_progress(const mavlink_openhd_wifbroadcast_scan_channels_progress_t& msg);
public:
    Q_INVOKABLE void fetch_channels_if_needed();

    Q_INVOKABLE bool start_analyze_channels();
    // freq_bands:
    // 0: 2.4G and 5.8G
    // 1: 2.4G only
    // 2: 5.8G only
    // similar for channel widths
    Q_INVOKABLE bool start_scan_channels(int freq_bands,int channel_widths);
public:
    void validate_and_set_channel_mhz(int channel);
    void validate_and_set_channel_width_mhz(int channel_width_mhz);
    void after_channel_freq_or_channel_width_check_ready();

    static constexpr auto PARAM_ID_WB_FREQ=openhd::WB_FREQUENCY;
    static constexpr auto PARAM_ID_WB_CHANNEL_WIDTH=openhd::WB_CHANNEL_WIDTH;
    static constexpr auto PARAM_ID_WB_MCS_INDEX=openhd::WB_MCS_INDEX;

    // Air and ground should always match, otherwise something weird has happenened.
    // Note that this would be "really" weird, since on not matching params there should be no connectivitiy.
    Q_INVOKABLE int get_param_int_air_and_ground_value(QString param_id);
    Q_INVOKABLE int get_param_int_air_and_ground_value_freq(){
        return get_param_int_air_and_ground_value(PARAM_ID_WB_FREQ);
    }
    Q_INVOKABLE int get_param_int_air_and_ground_value_channel_width(){
        return get_param_int_air_and_ground_value(PARAM_ID_WB_CHANNEL_WIDTH);
    }

    void change_param_air_and_ground(QString param_id,int value,bool allow_changing_without_connected_air_unit,bool log_to_hud=false);

    Q_INVOKABLE void change_param_air_and_ground_frequency(int value){
        QSettings settings;
        const bool qopenhd_allow_changing_ground_unit_frequency_no_sync = settings.value("qopenhd_allow_changing_ground_unit_frequency_no_sync",false).toBool();
        change_param_air_and_ground(PARAM_ID_WB_FREQ,value,qopenhd_allow_changing_ground_unit_frequency_no_sync);
    }

    Q_INVOKABLE void change_param_air_and_ground_channel_width(int value,bool log_to_hud=false){
        QSettings settings;
        const bool qopenhd_allow_changing_ground_unit_channel_width_no_sync = settings.value("qopenhd_allow_changing_ground_unit_channel_width_no_sync",false).toBool();
        change_param_air_and_ground(PARAM_ID_WB_CHANNEL_WIDTH,value,qopenhd_allow_changing_ground_unit_channel_width_no_sync,log_to_hud);
    }

    //
    Q_INVOKABLE int get_next_supported_frequency(int index);
    Q_INVOKABLE QString get_frequency_description(int frequency_mhz);
private:
    void log_result_message(const std::string& result_message,bool use_hud);
private:
    std::unique_ptr<RequestMessageHelper> m_request_message_helper;
    std::vector<uint16_t> m_supported_channels;
    void update_channels_on_success();
    bool m_valid_channel_channel_width_once=false;
public:
    Q_INVOKABLE void analyze_channels();
};

#endif // SynchronizedSettings_H
