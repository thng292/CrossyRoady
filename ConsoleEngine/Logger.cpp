#include "Logger.h"

std::ostream* Logger::out = &std::cerr;

void Logger::Init(std::ostream* out) { Logger::out = out; }

