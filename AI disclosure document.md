AI Disclosure: IPv4 Extraction Assignment  
1\. General Disclosure

* AI tool used: Claude Sonnet 5 (Anthropic), through the claude.ai chat interface  
* Date(s) consulted: September 27, 2026   
* Language: C++ (ipv4\_extract.cpp)  
* Ways the tool was used: explaining the assignment, reviewing my parsing logic, generating the program code, explaining how to format and run a test file, and generating a script to run the tests

**2\. Code Attribution**  
2.1 My account of how I used the AI  
I use Claude Sonnet 5 to clarify and summarize the objective of the assignment, what the assignment requires as deliverables and the structure of the code needed to deliver. Which is write C or C++ code that processes a line of input to determine a valid IPv4 address and state it out or to notify that the input does not contain any valid IPv4 address. The assignment stated the fixed grammar that is 4 octets 0 to 255, no leading zeros and optional port 0-65535 etc. Also identify all libraries, functions that are not allowed to use with additional constraints.  
The following is my personal reasoning behind what the code should do before asking an AI tool to complete the syntax: “The code would require an array to process the input, and a pointer to point to individual values in the array. There must be a way to identify the octet when reading through this array, a parser is the method to interpret the values, but how would a parser work? When a parser must be able to identify the valid from invalid, when meet invalid value the parser would ignore it and restart the array since it is now invalid. Conditions to which it restarts would be wrong octet count, empty octet, out-of-range octet or port, and does not allow leading 0 except if everything is 0\. I think I must write a function that assists the parser to identify the patterns of invalid or garage strings. Then the function can print the valid IPv4 or notify invalid input was given.” And I prompted: check my reasoning for the program if it is correct or not.  
Claude Sonnet 5 agrees with my approach with minor correction: when encountering a invalid character, pointers have to continue to go through the array and not start over because if it starts over the program would look at the same invalid input. After receiving feedback, my prompt was “based on your feedback, and the code logic I provided, write the complete program which follows the logic I provided”.  
I review the code and add additional comments to what the function goal is and what they do. I tested the code and it runs without error, then using the sample run provided in the assignment description to test and the program passed all those tests. I decided to write a test file to feed the program so I can write as many test cases as I want without editing them individually. I then ask Claude to show me the format that the program can read, then ask Claude to provide me with a bash script to run the test cases file “how to format the tests.txt so the program can properly process it?” Then I asked Claude to “reformat this documentation to not change what I wrote but to complete the documentation that meets the spec which requires the specific prompt.”

2.2 Exact prompts in order  
The assignment text was pasted into the chat with prompt 1\.

1. help me understand this assignment and what to do from start to finish.  
2. so what i need to build is C code that process a chain of input and determine if that chain of input is valid or not depending on the rule in which i create. is that correct?  
3. Here is my logic for the code, check for accuracy and correct things i miss: followed by my reasoning quoted in section 2.1 ("The code would require an array to process the input... notify invalid input was given.")  
4. how to format the tests.txt so the program can properly process it?  
5. can i just keep the test file as is? and submit the assignment that way?  
6. reformat this documentation to not changes what i wrote but to complete the documentation that meets the spec which requires the specific prompt: followed by the text in section 2.1

2.3 What was AI-generated and what was written by me

| Item | Author |
| ----- | ----- |
| Design reasoning for the parser (section 2.1) | Me |
| extractIPv4, validateRun, readNumber, isTokenChar in ipv4\_extract.cpp | Claude Sonnet 5, generated from my logic |
| main() input loop and output formatting | Claude Sonnet 5 |
| Comments in ipv4\_extract.cpp | Claude Sonnet 5 wrote initial comments; I reviewed them and add additional comment and make changes to Sonnet comment. |
| tests.txt | Me |
| Explanation of the test file format and run\_tests.sh | Claude Sonnet 5 |
| Structure and formatting of this disclosure document | Claude Sonnet 5; the text in section 2.1 is my own wording, unchanged |

Claude also generated additional challenge test cases and ran its own extra checks. These were **not** used in my submitted tests.

2.4 Feedback the AI gave on my reasoning  
Claude confirmed the overall approach (walk a character array with a pointer, use a helper function to validate) and corrected these points:

* The scanner should **not restart from the beginning** after an invalid token. It should resume after the failed section, otherwise it would keep finding the same invalid input.  
* Garbage is simply any character that is not a digit, period, or colon. It is skipped and it splits the input into separate candidate tokens.  
* My list of invalid conditions was missing: octets longer than 3 digits and ports longer than 5 digits, a colon that is not right after the fourth octet, a second colon, a stray period or colon attached to an address, and an empty port after a colon.  
* A leading zero is only allowed for the single digit 0; 00 and 000 are invalid.  
* extractIPv4 should not print anything. It returns success or failure and main does all the printing.

2.5 Modifications I made to the AI-generated code  
I made no changes to the program logic. I read and reviewed the code and added or edited comments so each function's purpose was clear to me.

**3\. Testing and Results**  
I tested the program first with the sample run from the assignment description, and it matched every output. I then wrote tests.txt so many cases can be fed to the program at once (./ipv4 \< tests.txt). Each line is one input and the last line is END. A small bash script, run\_tests.sh, prints each input next to its result.  
| # | Input | Result | Pass |
|---|---|---|---|
| 1 | `connecting to 192.168.1.1 now` | 192.168.1.1, value 3232235777, port none | Yes |
| 2 | `server=10.0.0.255:8080end` | 10.0.0.255, value 167772415, port 8080 | Yes |
| 3 | `chicken 172.197.2.2nugget ` | 172.197.2.2, value 2898592258, port none | Yes |
| 4 | `12bsa168.1.1.1` | 168.1.1.1, value 2818638081, port none | Yes |
| 5 | `192.168.1.0.` | Invalid (trailing period) | Yes |
| 6 | `0` | Invalid (not four octets) | Yes |
| 7 | `0.274.1.2` | Invalid (octet out of range) | Yes |
| 8 | `111.112.113.114` | 111.112.113.114, value 1869640050, port none | Yes |
| 9 | `192.168.01.1` | Invalid (leading zero) | Yes |
| 10 | `sigma67.67.67.2` | 67.67.67.2, value 1128481538, port none | Yes |
| 11 | `1.2.3.4:99999` | Invalid (port out of range) | Yes |
| 12 | `12.34.56` | Invalid (three octets) | Yes |
| 13 | `no number here` | Invalid (no address) | Yes |
| 14 | `0.0.0.0` | 0.0.0.0, value 0, port none | Yes |
| 15 | `255.255.255.255:65535` | 255.255.255.255, value 4294967295, port 65535 | Yes |
| 16 | `1.2.3.4:` | Invalid (empty port) | Yes |
| 17 | `1.2.3.4::80` | Invalid (second colon) | Yes |
| 18 | `256.1.1.1` | Invalid (octet out of range) | Yes |
| 19 | `hello0.12.34.33.1` | Invalid (five octets; the letters do not split the number run, so the whole run is rejected) | Yes |
| 20 | `hello192.168.1.0` | 192.168.1.0, value 3232235776, port none | Yes |
| 21 | `end` | Invalid (lowercase `end` is not the terminator because `END` is case-sensitive) | Yes |
| 22 | (empty line) | Invalid (no address) | Yes |
| | `END` | Program terminated. | Yes |

Raw output from `run_tests.sh`:

```
[connecting to 192.168.1.1 now]  -> Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)
[server=10.0.0.255:8080end]      -> Extracted IPv4 address: 10.0.0.255 (decimal value: 167772415, port: 8080)
[chicken 172.197.2.2nugget ]     -> Extracted IPv4 address: 172.197.2.2 (decimal value: 2898592258, port: none)
[12bsa168.1.1.1]                 -> Extracted IPv4 address: 168.1.1.1 (decimal value: 2818638081, port: none)
[192.168.1.0.]                   -> Invalid input: no valid IPv4 address found
[0]                              -> Invalid input: no valid IPv4 address found
[0.274.1.2]                      -> Invalid input: no valid IPv4 address found
[111.112.113.114]                -> Extracted IPv4 address: 111.112.113.114 (decimal value: 1869640050, port: none)
[192.168.01.1]                   -> Invalid input: no valid IPv4 address found
[sigma67.67.67.2]                -> Extracted IPv4 address: 67.67.67.2 (decimal value: 1128481538, port: none)
[1.2.3.4:99999]                  -> Invalid input: no valid IPv4 address found
[12.34.56]                       -> Invalid input: no valid IPv4 address found
[no number here]                 -> Invalid input: no valid IPv4 address found
[0.0.0.0]                        -> Extracted IPv4 address: 0.0.0.0 (decimal value: 0, port: none)
[255.255.255.255:65535]          -> Extracted IPv4 address: 255.255.255.255 (decimal value: 4294967295, port: 65535)
[1.2.3.4:]                       -> Invalid input: no valid IPv4 address found
[1.2.3.4::80]                    -> Invalid input: no valid IPv4 address found
[256.1.1.1]                      -> Invalid input: no valid IPv4 address found
[hello0.12.34.33.1]              -> Invalid input: no valid IPv4 address found
[hello192.168.1.0]               -> Extracted IPv4 address: 192.168.1.0 (decimal value: 3232235776, port: none)
[end]                            -> Invalid input: no valid IPv4 address found
[]                               -> Invalid input: no valid IPv4 address found
```

4\. Verification Statement

* I confirm that I understand every line of the submitted code and did not copy it blindly. I confirm that the code has been tested with the assignment's sample run and the test cases above, and it works as intended for them.  
* Known behaviors and limitations:  
  * If a line contains more than one valid address, the program returns the **first** one found.  
  * If an invalid token is followed by a valid one on the same line (for example 1.2.3.4:99999 5.6.7.8), the invalid token is skipped and the valid one is returned.  
  * A colon or period directly attached to an address makes that token invalid, so input like IP:1.2.3.4 is rejected. I followed the assignment's rule about stray punctuation literally.  
  * Characters that are not digits, periods, or colons (such as \- or \[) are treated as garbage, so \-1.2.3.4 returns 1.2.3.4.  
  * If the input file uses Windows line endings, the final END line is not recognized, so the program ends at the end of the file without printing "Program terminated."  
  * My test list covers the cases above but is not exhaustive.

