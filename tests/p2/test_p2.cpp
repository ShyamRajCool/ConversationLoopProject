// tests/p2/test_p2.cpp
//
// YOUR test suite goes here. At least 12 assert-based test cases — see
// spec §5 for the required categories and the sample test for the
// expected level of rigor.
//
// This file is a stub so the project builds out of the box; replace the
// body of main() with your own tests.

#include "core/conversation.h"
#include "core/message.h"
#include "core/sentinel_scanner.h"
#include "harness/harness.h"
#include "model/replay_client.h"
#include "model/scripted_client.h"
#include <cassert>

void empty_conversation_bounds() {
    Conversation new_conversation;
    Message holder;

    std::size_t empty = 0;

    assert(new_conversation.begin() == new_conversation.end()); //nullptr == nullptr
    assert(new_conversation.size() == empty);

    bool error_0_index = false;
    bool error_2_index = false;
    bool error_10_index = false;

    try {
        holder = new_conversation.at(0);
    } 
    catch (const std::out_of_range&){

        error_0_index = true;

    }

    assert(error_0_index && "evaluating at the 0th index of new_conversation");

    try {
        holder = new_conversation.at(2);
    } 
    catch (const std::out_of_range&){

        error_2_index = true;

    }

    assert(error_2_index && "evaluating at the 2nd index of new_conversation");

    try {
        holder = new_conversation.at(10);
    } 
    catch (const std::out_of_range&){

        error_10_index = true;

    }

    assert(error_10_index && "evaluating at the 10th index of new_conversation");
}

/*
void SysMessages(){

}
I was unsure of how to do this without implementing a linked list, which defeats the purpose of having the dynamically allocated array; I forgot this requirement of the spec when I was creating the original code
*/

void copyConstructor(){

    //assuming only copy constructor, not assignment operator
    Conversation current;
    std::string new_string = "Hello";
    Message new_message(Role::User,new_string);
    current.append(new_message);

    Conversation copy_conversation = current;

    const Message* ptr1 = copy_conversation.begin(); //This functions like a getter
    const Message* ptr2 = current.begin(); //This also functions like a getter

    std::size_t size_ptr1 = copy_conversation.size();
    std::size_t size_ptr2 = current.size();


    assert(ptr1 != ptr2 && " an assertion between Message* ptr1 and ptr2");
    assert(size_ptr1 == size_ptr2 && " sizes are equal");


}

void moveConstructor(){
    Conversation current;
    std::string new_string = "Hello";
    Message new_message(Role::User,new_string);
    current.append(new_message);

    const Message* ptr_old = current.begin();
    std::size_t old_size = current.size();

    Conversation move_conversation(std::move(current));

    const Message* ptr1 = move_conversation.begin(); //This functions like a getter and it should be ptr_old
    const Message* ptr2 = current.begin(); //This also functions like a getter and should be nullptr

    
    std::size_t size_ptr1 = move_conversation.size();
    std::size_t size_ptr2 = current.size();
    
    assert(ptr1 == ptr_old && "an assertion between the old pointer and the new move_conversation pointer");
    assert(ptr1 != ptr2 && "an assertion between the old pointer and nullptr");


    assert(old_size == size_ptr1 && "sizes are equal");
    assert(size_ptr1 != size_ptr2 && "sizes are not equal");

}

void growth(){
    Conversation current;

    std::string new_string = "Hello";
    std::size_t num_cap = 0; //capacity changes
    const Message* memspot = nullptr;
    
    for (std::size_t i = 0; i < 10; i++){
        //since we can, but the spec doesn't specify to create a capcacity getter, I am just using pointer arithmetic
        std::string temp_string = new_string + static_cast<char>(i+1 % 20);
        Message new_message(Role::User,temp_string);

        if (i != 0 ){
            Message temp_message = current.at(i/2); //This will get 0,1,2,3...5 without accessing out of bounds
            std::string temp_contents = temp_message.content();

            current.append(new_message);

            assert(current.size() == i+1);

            const Message* initialmem = current.begin();

            if (initialmem != memspot || i == 0) { //means an realloc happened
                memspot = initialmem;
                num_cap = num_cap + 1;
            }

            Message curr_message = current.at(i/2);
            std::string curr_contents = curr_message.content();

            assert(curr_contents == temp_contents);
        }
        else{
            current.append(new_message);

            assert(current.size() == i+1);

            const Message* initialmem = current.begin();

            if (initialmem != memspot || i == 0) { //means an realloc happened
                memspot = initialmem;
                num_cap = num_cap + 1;
            }
        }
        
    }
    assert(num_cap == 5);
}

void cleansentinel(){ //this test is from lines 341 to 350 of the spec; credit to TA Sina
                      //doubles as splitting at every possible boundary

    const std::string sentinel = "<|end_conversation|>";
    const std::string text = "Goodbye.";
    for (std::size_t split = 0; split <= text.size(); ++split) {
        SentinelScanner scanner(sentinel);
        auto out1 = scanner.feed(text.substr(0, split));
        auto out2 = scanner.feed(text.substr(split));
        auto out3 = scanner.flush();
        assert((!out1.sentinel_found && !out2.sentinel_found && !out3.sentinel_found) &&
               "sentinel shouldn't be found");
        assert(out1.safe_text + out2.safe_text +out3.safe_text == "Goodbye.");
    }
}

void splitsentinel(){ //this test is from lines 341 to 350 of the spec; credit to TA Sina
                      //doubles as splitting at every possible boundary
    const std::string sentinel = "<|end_conversation|>";
    const std::string text = "Goodbye." + sentinel;
    for (std::size_t split = 0; split <= text.size(); ++split) {
        SentinelScanner scanner(sentinel);
        auto out1 = scanner.feed(text.substr(0, split));
        auto out2 = scanner.feed(text.substr(split));
        assert((out1.sentinel_found || out2.sentinel_found) &&
               "sentinel must be caught regardless of split point");
        assert(out1.safe_text + out2.safe_text == "Goodbye.");
    }
}

void falsealarm(){ //this test is from lines 341 to 350 of the spec; credit to TA Sina
                    //doubles as splitting at every possible boundary
    const std::string sentinel = "<|end_conversation|>";
    const std::string text = "New <|end_conversation| place text.";
    for (std::size_t split = 0; split <= text.size(); ++split) {
        SentinelScanner scanner(sentinel);
        auto out1 = scanner.feed(text.substr(0, split));
        auto out2 = scanner.feed(text.substr(split));
        assert((!out1.sentinel_found && !out2.sentinel_found) &&
               "partial match shouldn't trigger");
        assert(out1.safe_text + out2.safe_text == text);
    }
}


/*I'm not sure how to implement case 9 as we can't access pending_'s size without creating a getter function
*/

/*I'm not sure how test 10 and 11 work since I don't have experience with using any of the Harness file code */

/*I wasn't sure of how to get test 12 working and what elements are needed

void save_transcript(){

    std::string file_name = "conversation.txt";

    Conversation new_conv;
    new_conv.append(Message(Role::System, "Hello World"));
    new_conv.append(Message(Role::User, "Hello System"));
    new_conv.append(Message(Role::Assistant, "I am Assistant, not System"));
    new_conv.append(Message(Role::User,"Ok, nice"));
    new_conv.append(Message(Role::Assistant,"Yes very nice"));
    new_conv.append(Message(Role::User,"Done"));
    new_conv.append(Message(Role::Assistant, "Goodbye!<|end_conversation|>"));


}
*/


int main() {
    empty_conversation_bounds();
    copyConstructor();
    moveConstructor();
    growth();
    cleansentinel();
    splitsentinel();
    falsealarm();
    
    return 0;
}
