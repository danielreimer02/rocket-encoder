# rocket-encoder
A proof of concept for a character encoder to encoded two characters as one. 

The combine function combines all input data into a semi-colon separated character array. The function also replaces periods with '<' because in ASCII periods have a lower index than integers, so if the encode function subtracts by '0' there can be no characters with a lower index than '0'.

The encode function iterates through two characters at a time and subtracts the '0' character, from each of them, so that each character can be represented by four bits. The first character, of the two, is then bit shifted four to the left. The first and second characters are combined using bitwise or.  
