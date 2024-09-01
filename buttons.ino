void ring_check() {
  if (status_ring_button) {
    status_ring_button = false;
    state_ring = true;
  }

  static unsigned long temp_millis = millis();
  if ( millis() - temp_millis > 1 * 1000 ) {
    temp_millis = millis();
    Serial.print("state_ring "); Serial.print(state_ring);
    Serial.print("  status_ring_button "); Serial.println(status_ring_button);
  }
}


void send_messege_to_telegram() {
  bot.sendMessage("🔔 В дверь звонят 🔔: " + GetTime(), CHAT_ID);
  state_ring = false;
  write_log("звонок: " + GetDate() + GetTime());
}
