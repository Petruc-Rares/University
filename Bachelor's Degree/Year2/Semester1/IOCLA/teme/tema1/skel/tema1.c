#include <unistd.h>
#include <stdarg.h>
#include <string.h>


char *utoa(unsigned value, char *digits, int base)
{
    char *s, *p;

    s = "0123456789abcdefghijklmnopqrstuvwxyz"; /* don't care if s is in
                                                 * read-only memory
                                                 */
    if (base == 0)
        base = 10;
    if (digits == NULL || base < 2 || base > 36)
        return NULL;
    if (value < (unsigned) base) {
        digits[0] = s[value];
        digits[1] = '\0';
    } else {
        for (p = utoa(value / ((unsigned)base), digits, base);
             *p;
             p++);
        utoa( value % ((unsigned)base), p, base);
    }
    return digits;
}   

char *itoa(int value, char *digits, int base)
{
    char *d;
    unsigned u; /* assume unsigned is big enough to hold all the
                 * unsigned values -x could possibly be -- don't
                 * know how well this assumption holds on the
                 * DeathStation 9000, so beware of nasal demons
                 */

    d = digits;
    if (base == 0)
        base = 10;
    if (digits == NULL || base < 2 || base > 36)
        return NULL;
    if (value < 0) {
        *d++ = '-';
        u = -value;
    } else
        u = value;
    utoa(u, d, base);
    return digits;
}


static int write_stdout(const char *token, int length)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

int iocla_printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);

	const char* it;
	const char *auxString;
	// buffer-ul de size 34 biti pentru ca 32 pot ocupa cifrele
	// 1 pentru semn si 1 pentru '\0'
	char buffer[34];
	int auxNumber;
	int noCharacters = 0;
	char auxCharacter;
	char sizeChar = sizeof(char);

	for (it = format; *it != '\0'; it++) {
		// s-ar putea sa avem fie specificator
		// de conversie, fie '%%' ce in urma
		// afisarii printf, afiseaza %
		if ((*it) == '%') {
			it++;
			switch(*it) {
				case 'd':
					auxNumber = va_arg(args, int);
					itoa(auxNumber, buffer, 10);
					write_stdout(buffer, strlen(buffer));
					noCharacters += strlen(buffer);
					break;
				case 'u':
					auxNumber = va_arg(args, unsigned int);
					utoa(auxNumber, buffer, 10);
					write_stdout(buffer, strlen(buffer));
					noCharacters += strlen(buffer);
					break;
				case 'c':
					auxCharacter =  va_arg(args, int);
					write_stdout(&auxCharacter, sizeChar);
					noCharacters += sizeChar;
					break;
				case 'x':
					auxNumber = va_arg(args, int);
					utoa(auxNumber, buffer, 16);
					write_stdout(buffer, strlen(buffer));
					noCharacters += strlen(buffer);
					break;
				case 's':
					auxString = va_arg(args, const char*);
					write_stdout(auxString, strlen(auxString));
					noCharacters += strlen(auxString);
					break;
				case '%':
					write_stdout("%", sizeChar);
					noCharacters += sizeChar;
					break;
			}
		} else {
			noCharacters += sizeChar;
			write_stdout(it, sizeChar);
		}
	}
	return noCharacters;
}
