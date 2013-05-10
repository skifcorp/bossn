#ifndef WARNMESSAGES_H
#define WARNMESSAGES_H

#include <QString>

static const char * greeting_message                       = QT_TRANSLATE_NOOP("MainSequence", "Go on the weight platfotrm");
static const char * apply_card_message                     = QT_TRANSLATE_NOOP("MainSequence", "Apply card to reader");
static const char * card_autorize_error_message            = QT_TRANSLATE_NOOP("MainSequence", "Card autorization error!");
static const char * card_reading_error_message             = QT_TRANSLATE_NOOP("MainSequence", "Card reading error!");
static const char * card_is_empty_error_message            = QT_TRANSLATE_NOOP("MainSequence", "Card is empty error!");
static const char * car_blocked_message                    = QT_TRANSLATE_NOOP("MainSequence", "You car is blocked! Contact to dispatcher!");
static const char * fetch_car_error_message                = QT_TRANSLATE_NOOP("MainSequence", "Fetch car error! Contact to dispatcher");
static const char * fetch_ttn_error_message                = QT_TRANSLATE_NOOP("MainSequence", "Fetch ttn error! Contact to dispatcher");
static const char * weight_not_stable_message              = QT_TRANSLATE_NOOP("MainSequence", "Weights dont stable!");
static const char * brutto_rupture_failed_message          = QT_TRANSLATE_NOOP("MainSequence", "Brutto rupture to big!");
static const char * update_ttn_error_message               = QT_TRANSLATE_NOOP("MainSequence", "Update ttn error! Contact to dispatcher");
static const char * autodetect_platform_type_error_message = QT_TRANSLATE_NOOP("MainSequence", "Platform autodetect failed! Contact to dispatcher");
static const char * kvo_mah_error_message                  = QT_TRANSLATE_NOOP("MainSequence", "KvoMah failed! Contact to dispatcher");
static const char * kontr_get_error_message                = QT_TRANSLATE_NOOP("MainSequence", "KontrGet failed! Contact to dispatcher");
static const char * get_car_for_bum_error_message          = QT_TRANSLATE_NOOP("MainSequence", "Getting car for bum failed! Contact to dispatcher");
static const char * confuse_brutto_tara_error_message      = QT_TRANSLATE_NOOP("MainSequence", "You confused brutto with tara! Contact to dispatcher");
static const char * get_backboard_bum_weight_const_error   = QT_TRANSLATE_NOOP("MainSequence", "Getting backboard bum weight error! Contact to dispatcher");
static const char * get_free_bum_error                     = QT_TRANSLATE_NOOP("MainSequence", "Getting free bum error! Contact to dispatcher");
static const char * update_bum_queue_error                 = QT_TRANSLATE_NOOP("MainSequence", "Updating bum queue error! Contact to dispatcher");
static const char * forget_brutto_on_tara_error_message    = QT_TRANSLATE_NOOP("MainSequence", "You forget for brutto!");
static const char * get_ttn_by_driver_tara0_error_message  = QT_TRANSLATE_NOOP("MainSequence", "Get ttn by driver and zero tara failed! Contact to dispatcher");
static const char * get_last_ttn_by_driver_error_message   = QT_TRANSLATE_NOOP("MainSequence", "Get last ttn by driver failed! Contact to dispatcher");
static const char * brutto_smaller_than_tara_message       = QT_TRANSLATE_NOOP("MainSequence", "Brutto smaller than tara! Contact to dispatcher");
static const char * tara_rupture_failed_message            = QT_TRANSLATE_NOOP("MainSequence", "Tara rupture to big!");
static const char * car_dont_was_in_lab                    = QT_TRANSLATE_NOOP("MainSequence", "Car was sent to lab has not been there!");
static const char * car_has_not_been_unloaded              = QT_TRANSLATE_NOOP("MainSequence", "Car has not been unloaded!");
static const char * blocking_car_for_lab_error             = QT_TRANSLATE_NOOP("MainSequence", "Cant block car which wasnt in lab!");
static const char * getting_kagat_error                    = QT_TRANSLATE_NOOP("MainSequence", "Cant get kagat!");
static const char * kagat_was_closed_error                 = QT_TRANSLATE_NOOP("MainSequence", "Kagat was closed!");
static const char * clear_bum_queue_error                  = QT_TRANSLATE_NOOP("MainSequence", "Clear bum queue error!");
static const char * cant_get_field_when_printing           = QT_TRANSLATE_NOOP("MainSequence", "Cant get field when printing");
static const char * cant_get_kontr_when_printing           = QT_TRANSLATE_NOOP("MainSequence", "Cant get kontr when printing");
static const char * cant_get_base_firm_when_printing       = QT_TRANSLATE_NOOP("MainSequence", "Cant get base firm when printing");
static const char * cant_get_bum_state_log_message         = QT_TRANSLATE_NOOP("MainSequence", "Cant get bum state log");
static const char * cant_get_bum_message                   = QT_TRANSLATE_NOOP("MainSequence", "Cant get bum!");
static const char * cant_get_const_message                 = QT_TRANSLATE_NOOP("MainSequence", "Cant get const");
static const char * error_getting_mid_tara_message         = QT_TRANSLATE_NOOP("MainSequence", "Error getting mid tara!");
static const char * error_writing_rup_tara_message         = QT_TRANSLATE_NOOP("MainSequence", "Error writing rup tara!");
static const char * perimeter_control_failed               = QT_TRANSLATE_NOOP("MainSequence", "Perimeter control failed!");
static const char * stealed_card_message                   = QT_TRANSLATE_NOOP("MainSequence", "You used stealed card!");
static const char * weights_dont_work                      = QT_TRANSLATE_NOOP("MainSequence", "Weights dont work!!");
static const char * error_make_new_task                    = QT_TRANSLATE_NOOP("MainSequence", "Cant make new task!");
static const char * error_database_lost                    = QT_TRANSLATE_NOOP("MainSequence", "Lost connection to database!!!");
static const char * processing_message                     = QT_TRANSLATE_NOOP("MainSequence", "Processing card!!!");
static const char * cant_use_database                      = QT_TRANSLATE_NOOP("MainSequence", "Cant use database!!!");
static const char * cant_retara_after_leaving_weights      = QT_TRANSLATE_NOOP("MainSequence", "Cant retara after leaving weights");
static const char * cant_rebrutto_after_leaving_weights    = QT_TRANSLATE_NOOP("MainSequence", "Cant rebrutto after leaving weights");
static const char * cant_fetch_prikaz                      = QT_TRANSLATE_NOOP("MainSequence", "Cant get prikaz");
static const char * prikaz_already_closed                  = QT_TRANSLATE_NOOP("MainSequence", "Prikaz already closed");
static const char * fake_ttn_creation_error                = QT_TRANSLATE_NOOP("MainSequence", "Fake ttn creation error");
static const char * cant_update_prikaz                     = QT_TRANSLATE_NOOP("MainSequence", "Cant update prikaz");
static const char * prikaz_creation_error                  = QT_TRANSLATE_NOOP("MainSequence", "Prikaz creation error");
static const char * car_netto_too_big_for_prikaz           = QT_TRANSLATE_NOOP("MainSequence", "Netto too big for your prikaz");
static const char * car_has_netto_overlimit                = QT_TRANSLATE_NOOP("MainSequence", "Car has netto overlimit");
static const char * cant_get_ttn_with_max_netto            = QT_TRANSLATE_NOOP("MainSequence", "Cant get ttn with max netto");
static const char * your_card_is_banned                    = QT_TRANSLATE_NOOP("MainSequence", "Your card is banned");
static const char * cant_get_virtual_netto                 = QT_TRANSLATE_NOOP("MainSequence", "Cant get virtual netto");
static const char * requested_constant_not_exists          = QT_TRANSLATE_NOOP("MainSequence", "Requested constant not exists");
static const char * connect_to_service_server_error        = QT_TRANSLATE_NOOP("MainSequence", "Connect to service server error");
static const char * connect_to_service_server_timeout      = QT_TRANSLATE_NOOP("MainSequence", "Connect to service server timeout");
static const char * read_from_service_server_error         = QT_TRANSLATE_NOOP("MainSequence", "Read from service server error");
static const char * read_from_service_server_timeout       = QT_TRANSLATE_NOOP("MainSequence", "Read from service server timeout");

static const char * gsoap_data_exchange_request_error      = QT_TRANSLATE_NOOP("MainSequence", "Gsoap data exchange request error");
static const char * gsoap_accept_card_result_request_error = QT_TRANSLATE_NOOP("MainSequence", "Gsoap accept card result request error request error");
static const char * web_service_wrong_reader_bytes_count   = QT_TRANSLATE_NOOP("MainSequence", "web service returned wrong number of bytes for reader");
static const char * web_service_corrupted_reader_byte      = QT_TRANSLATE_NOOP("MainSequence", "web service returned corruped reader byte");

static const char * brutto_finish_weight_message                  = QT_TRANSLATE_NOOP("MainSequence", "%1 kg");
static const char * brutto_finish_lab_message                     = QT_TRANSLATE_NOOP("MainSequence", "Lab(%1)");
static const char * brutto_finish_bum_message                     = QT_TRANSLATE_NOOP("MainSequence", "BUM %1");




inline void remove_unsued_warning()
{
    Q_UNUSED(greeting_message);
    Q_UNUSED(apply_card_message);
    Q_UNUSED(card_autorize_error_message);
    Q_UNUSED(card_reading_error_message);
    Q_UNUSED(card_is_empty_error_message);
    Q_UNUSED(car_blocked_message);
    Q_UNUSED(fetch_car_error_message);
    Q_UNUSED(fetch_ttn_error_message);
    Q_UNUSED(weight_not_stable_message);
    Q_UNUSED(brutto_rupture_failed_message);
    Q_UNUSED(update_ttn_error_message);
    Q_UNUSED(autodetect_platform_type_error_message);
    Q_UNUSED(kvo_mah_error_message);
    Q_UNUSED(kontr_get_error_message);
    Q_UNUSED(get_car_for_bum_error_message);
    Q_UNUSED(confuse_brutto_tara_error_message);
    Q_UNUSED(get_backboard_bum_weight_const_error);
    Q_UNUSED(get_free_bum_error);
    Q_UNUSED(update_bum_queue_error);
    Q_UNUSED(forget_brutto_on_tara_error_message);
    Q_UNUSED(get_ttn_by_driver_tara0_error_message);
    Q_UNUSED(get_last_ttn_by_driver_error_message);
    Q_UNUSED(brutto_smaller_than_tara_message);
    Q_UNUSED(tara_rupture_failed_message);
    Q_UNUSED(car_dont_was_in_lab);
    Q_UNUSED(car_has_not_been_unloaded);
    Q_UNUSED(blocking_car_for_lab_error);
    Q_UNUSED(getting_kagat_error);
    Q_UNUSED(kagat_was_closed_error);
    Q_UNUSED(clear_bum_queue_error);
    Q_UNUSED(cant_get_field_when_printing);
    Q_UNUSED(cant_get_kontr_when_printing);
    Q_UNUSED(cant_get_base_firm_when_printing);
    Q_UNUSED(cant_get_bum_state_log_message);
    Q_UNUSED(cant_get_bum_message);
    Q_UNUSED(cant_get_const_message);
    Q_UNUSED(error_getting_mid_tara_message);
    Q_UNUSED(error_writing_rup_tara_message);
    Q_UNUSED(perimeter_control_failed);
    Q_UNUSED(stealed_card_message);
    Q_UNUSED(weights_dont_work);
    Q_UNUSED(error_make_new_task);
    Q_UNUSED(error_database_lost);
    Q_UNUSED(processing_message);
    Q_UNUSED(cant_use_database);

    Q_UNUSED(cant_retara_after_leaving_weights);
    Q_UNUSED(cant_rebrutto_after_leaving_weights);

    Q_UNUSED(cant_fetch_prikaz);
    Q_UNUSED(prikaz_already_closed);
    Q_UNUSED(fake_ttn_creation_error);
    Q_UNUSED(cant_update_prikaz);
    Q_UNUSED(prikaz_creation_error);
    Q_UNUSED(cant_get_virtual_netto);
    Q_UNUSED(car_netto_too_big_for_prikaz);
    Q_UNUSED(car_has_netto_overlimit);
    Q_UNUSED(cant_get_ttn_with_max_netto);
    Q_UNUSED(your_card_is_banned);
    Q_UNUSED(brutto_finish_weight_message);
    Q_UNUSED(brutto_finish_lab_message);
    Q_UNUSED(brutto_finish_bum_message);

    Q_UNUSED(connect_to_service_server_error);
    Q_UNUSED(connect_to_service_server_timeout);
    Q_UNUSED(read_from_service_server_error);
    Q_UNUSED(read_from_service_server_timeout);

    Q_UNUSED(gsoap_data_exchange_request_error);
    Q_UNUSED(gsoap_accept_card_result_request_error);

    Q_UNUSED(requested_constant_not_exists);

    Q_UNUSED(web_service_wrong_reader_bytes_count);
    Q_UNUSED(web_service_corrupted_reader_byte);
}

#endif // WARNMESSAGES_H
