/*
 ============================================================================
 Name        : encode.c
 Author      : Daniel Reimer
 Version     :
 Description : Proof of Concept for character encoder
 Encodes two characters (because only integers, commas, and periods are needed)
 as one and then decodes them
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int combine(unsigned char *total, double accel, double accel2);
int encode(unsigned char *buffer, double accel, double accel2);
int decode(unsigned char *encoded, unsigned char *buffer);

int main(void) {

	//large buffer of arbitrary length
	//(has to be large enough to contain input data)
	unsigned char total[20];

	//data to encode
	double accel = 12345;
	double accel2 = 6789;

	//the encode function returns the
	//length of the encoded character array
	int encodedlen = encode(total, accel, accel2);


	printf("encoded length: %d\n\n", encodedlen);



	//create an array on the receiving side, which will hold the transmitted characters
	//arbitrary length but has to be large enough to contain encoded data
	unsigned char encoded[encodedlen];

	//mock sending through radio
	//the radio will only send the encoded characters
	for (int i = 0; i < encodedlen; i++) {

		//the encoded array holds the encoded characters, which were sent from the radio
		encoded[i] = total[i];
		printf("encoded: %c\n", encoded[i]);

	}

	fflush( stdout);




//the only information that the receiver has is the encoded character array




	//create arbitrary length array
	//has to be large enough to contain decoded data
	unsigned char *decoded[100];

	//decodes the encoded array and stores it in the decoded array
	decode(encoded, decoded);


	//can use strlen function now because everything in
	//the decoded array should be ASCII characters 0-9 or '.' or ';'
	int decodedlen = strlen(decoded);

	printf("decoded length is: %d\n\n", decodedlen);

	printf("decoded: %s\n\n", decoded);

	fflush( stdout);

}

//returns the length of the encoded character array
//the pointer to the decoded character array (which is passed into the function)
//will point to the encoded character array
int encode(unsigned char *buffer, double accel, double accel2) {

	//combine all inputs into one character array
	combine(buffer, accel, accel2);

	//declare variables
	int encodedlen;
	int bufferlen = strlen(buffer);

	//this project assumes that the decoded character array
	//will always contain an even number of characters
	//the encoded character array will be half
	//the length of the decoded character array
	encodedlen = (bufferlen / 2);

	//iterate through buffer and encode two characters at a time
	//store encoded characters back in buffer
	int j = 0;

	for (int i = 0; i < bufferlen; i += 2) {

		//subtract '0' so that each character can be represented by four bits
		buffer[i] = buffer[i] - '0';
		buffer[i + 1] = buffer[i + 1] - '0';

		//bit shift the first character
		buffer[i] = buffer[i] << 4;

		//add two characters together and store back in buffer
		buffer[j] = buffer[i] | buffer[i + 1];

		j++;
	}

	//the first half of buffer will contain the encoded characters

	return encodedlen;

}

//combines all data into one long character array and makes sure everything is
//0-9 or ; (to separate inputs) or < (to replace periods)--in ASCII periods are a lower
//index than integers so if encode wants to subtract by '0' there can be no characters with a lower index than '0'
//returns 0 if function completes
int combine(unsigned char *total, double accel, double accel2) {

	//print everything to long total char array
	sprintf(total, "%f", accel);
	sprintf(total + sizeof(double), "%c", ';');
	sprintf(total + sizeof(double) + sizeof(unsigned char), "%f", accel2);
	sprintf(total + sizeof(double) + sizeof(unsigned char) + sizeof(double),"%c", ';');

	int totallen = strlen(total);

	//replace periods
	for (int i = 0; i < totallen; i++) {
		if (total[i] == '.') {
			total[i] = '<';
		}
	}

	return 0;
}

//returns 0 if the function completes
int decode(unsigned char *encoded, unsigned char *buffer) {

	//declare variables
	int semicoloncount = 0;

	//iterate through the encoded character array until the
	//predetermined number of inputs has be iterated through (denoted by the number of semicolons)
	int j = 0;
	for (int i = 0; i < 100; i += 2) {

		//mask the first and second characters
		buffer[i + 1] = encoded[j] & 0xF;
		buffer[i] = encoded[j] & 0xF0;

		//bit shift the first character
		buffer[i] = buffer[i] >> 4;

		//add '0' back to the characters
		buffer[i] = buffer[i] + '0';
		buffer[i + 1] = buffer[i + 1] + '0';

		//replace '<' with '.'
		if (buffer[i] == '<') {
			buffer[i] = '.';
		}

		if (buffer[i + 1] == '<') {
			buffer[i + 1] = '.';
		}

		//increase the count of the semicolons if a semicolon is encountered
		//both the first and the second number in the buffer can't be semicolons so 'or' can be used
		if (buffer[i] == ';' | buffer[i + 1] == ';') {
			semicoloncount++;

		}


		//if the number of semicolons is greater than or equal to
		//the predetermined number (in this case two) break from the for loop
		if (semicoloncount == 2) {
			break;
		}

		j++;

	}


	return 0;
}
