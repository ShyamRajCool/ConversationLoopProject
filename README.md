# Conversation Loop Project

This project was created for ECE 309 Project 2. There was a read me file and specifications file for me to follow in order to create this project. This project showcases using rule of five, getters, and dynamic array allocation for the purposes of creating a mock mini-LLM. The files created/edited/and published were conversation.h, message.h, sentinel_scanner.h, conversation.cpp, sentinel_scanner.cpp, test_p2.cpp, and design-log-p2.md. This project is a part of the semester long project in the ECE 309 Class at North Carolina State University. To use this, a greeting script is useful with the following:

role: system
Be concise.

chunk: 5
role: assistant
I am doing well, thank you! How can I help you?

chunk: 7
role: assistant
I can definitely do that for you. Anything else?

chunk: 6
role: assistant
Goodbye!<|end_conversation|>

Then using:
cmake -S . -B build
cmake --build build

and then 

./build/miniharness --script scripts/greeting.script --save transcript.txt

all within a linux terminal will allow you to interact with the program. To close the conversation early, press Ctrl-D on an empty line.
