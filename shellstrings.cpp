#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <algorithm>

std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

std::string make_push(std::string s, int stack_size, bool bits64) {
    std::reverse(s.begin(), s.end()); //reverse it so we can
                                      //properly push to the stack
    std::string full = "";
    //deal with non multiples of stack_size
    //There's potentially a better way to deal with the %stack_size=2 case,
    //but it requires knowing of a free register
    if (s.length() % stack_size != 0) {
        while (s.length() % stack_size != 0) {
            std::string push = "";
            if (!bits64) push = "push 0x";
            else push = "mov rax, 0x";
            int remainder = s.length() % stack_size;
            s.insert(0, (stack_size - remainder), 'a'); //Fill front with a's
            std::string currchar = s.substr(0, stack_size);
            push.append(string_to_hex(currchar));
            push.append("\t; '");
            push.append(currchar);
            push.append("'\n");
            full.append(push);

            if (bits64) full.append("push rax\n");

            //Insert subs
            for (int i = 0; i < (stack_size - remainder); i++) {
                std::string sub = "";
		if (!bits64) sub = "sub dword [esp + 0x";
		else sub = "sub dword [rsp + 0x";
                std::ostringstream oss;
                oss << (stack_size - i - 1);
                sub.append(oss.str());
                sub.append("], 0x61 ; sub 'a' to null\n");
                full.append(sub);
            }
            s = s.substr(stack_size);
        }
    }
    while (s.length() > 0) {
        std::string push = "";
        if (!bits64) push = "push 0x";
        else push = "mov rax, 0x";
        std::string currchar = s.substr(0, stack_size);
        push.append(string_to_hex(currchar));
        s = s.substr(stack_size);
        push.append("\t; '");
        push.append(currchar);
        push.append("'\n");
        full.append(push);

        if (bits64) full.append("push rax\n");
    }
    return full;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s [string]\n", argv[0]);
    }
    else if (argc == 2) {
        printf("%s\n", make_push(argv[1], 4, false).c_str());
    }
    else if (argc == 3) {
        std::string argv2 = argv[2];
        if (argv2 == "-x64") {
            printf("%s\n", make_push(argv[1], 8, true).c_str());
        }
    }
    else if (argc == 4) {
        std::string argv2 = argv[2];
        if (argv2 == "-s") {
            int stack_size = atoi(argv[3]);
            printf("STACK SIZE: %d\n", stack_size);
            printf("%s\n", make_push(argv[1], stack_size, false).c_str());
        }
    }
    return 1;
}
