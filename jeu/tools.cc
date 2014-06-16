// Copyright 2014 Huns de Troyes
#include "include/tools.h"

// Printf dans un string alloué dynamiquement
char* VprintfToNewString(const char* format, va_list args) {
	va_list args_copy;
	int length;
	char* buffer;
	int result;

	va_copy(args_copy, args);
	length = vsnprintf(NULL, 0, format, args);
	buffer = (char*)malloc(length + 1); /* +1 for NULL-terminator. */
	result = vsnprintf(&buffer[0], length + 1, format, args_copy);
	if (result != length) {
		assert(0);
		return NULL;
	}
	return buffer;
}

// Fonctions de debug, fonctionne comme un printf mais envoie un message au JS
void PostMessage(const char *format, ...) {
	char* string;
	va_list args;

	va_start(args, format);
	string = VprintfToNewString(format, args);
	va_end(args);

	PSInterfaceMessaging()->PostMessage(PSGetInstanceId(),
										PSInterfaceVar()->VarFromUtf8(string, strlen(string)));
	free(string);
}

// Determine la distance entre 2 points
float Dist(struct PP_FloatPoint from, struct PP_FloatPoint to) {
	return sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
}

// Cette fonction bloque un nombre entre 2 limites
float clamp(float val, float low, float high) {
	return fmin(fmax(val, low), high);
}
