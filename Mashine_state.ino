void mashine_state() {
  static unsigned long http_millis = millis();
  if ( millis() - http_millis > 5 * 60 * 1000 )
  {
    http_millis = millis();
    is_http_enable = false;
    if (is_AP_mode) is_http_enable = true;
  }

  static unsigned long wifi_ap_mode_millis = millis();
  if ( millis() - wifi_ap_mode_millis > 5 * 60 * 1000 ) {
    wifi_ap_mode_millis = millis();
    if (WiFi.status() != WL_CONNECTED or is_AP_mode) {
      wifi_start();
    }
    Time_init();
    //    Serial.print("is_http_enable ");
    //    Serial.println(is_http_enable);
  }

  static unsigned long ring_millis = millis();
  if ( millis() - ring_millis > 10 * 1000 ) {
    if (status_ring_button and send_ring == false) {
      ring_millis = millis();
      static unsigned long last_message_ring_millis = millis();
      Serial.println("RING");

      send_messege_to_telegram();

      //    String sticker = F("CAACAgIAAxkBAAEE6TBimvrjTXgKQyhdMBxohA_HZ6VtjAACTAADr8ZRGjrfxZE8LKM_JAQ");
      //    bot.sendSticker(sticker, CHAT_ID);

      //    } else {
      //      status_ring_button = false;
      //      state_ring = true;
    }
  }

  static unsigned long blink_millis = millis();
  if (blink) {
    if ( millis() - blink_millis > 1 * 1000 ) {
      blink_millis = millis();
      led_state = !led_state;
      digitalWrite(LED_PIN, led_state);
    }
  }
}
