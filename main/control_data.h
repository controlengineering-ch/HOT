#pragma once

void set_control_params(int polling_ms, int warn, int danger);

int get_polling_interval();
int get_warn_threshold();
int get_danger_threshold();

