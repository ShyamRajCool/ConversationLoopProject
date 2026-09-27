# Design Log — Project 2

(500–800 words total. See spec §5 for what each section must cover.)

## Growth factor and amortized cost

The growth factor chosen was 2 because the Appendix C provided insight into the rationale behind this. Citing Appendix C, the doubling of the capacity_ was useful since the number of appends happened as the size grew from 0 -> 1 -> 2 -> 4 -> 8 -> 16 and so on until max turns was reached. The doubling allows the same number of appends in the future that were present before an reallocation occurred. This allows an O(n) (O(capacity_)) append to occur every time the capacity_ needs to be doubled through reallocation. The total copying across n is O(n) and not O(n^2) - O(1) per append, which is what would be the case if it were a fixed growth of +1. The append size could be larger, and depending on the max turns, there is an optimal/valid factor for this reallocation. For this case, in which there aren't many messages being sent and replied, the growth factor of 2 is reasonable. 


## Rule of Five evidence
The rule of five is implemented because the copy constructor performs a deep copy with new dynamically allocated array for data_ each time that it runs out of space. The copy constructor correctly copies the capacity_, elements of data_, and the size_ without deleting or altering the object to be copied's contents. The same can be said about the assignment operator. The original object's contents for data_ are deleted, invoking the destructor, and peforming a deep copy of the object to be copied's contents. Since the copy assignment may require chaining, *this is returned.

The move constructor peforms the steal and resets the object to stolen's contents, which ensures that incorrect accessing of the stolen contents doesn't occur.

The move assignment ensures that an object isn't being copied to itself, which is wasteful of time. Like the move constructor, a steal is performed and the object to stolen's contents are restored. Like the assignment operator, *this is returned so that chaining can occur, if required.

The destructor will need to destroy data_ (which is an array), so this was implemented. It also does a lot of things in the background for cleaning up temporary variables and other things that go out of scope. 

These are all evident when I tried using them in the test cases.


## Sentinel scanner: bounded pending_ proof
In the sentinel_scanner.cpp file, the pending_ private variable has two cases where it is given text. Both of these cases only activate when the sentinel_ isn't found. The first case is where the chunk + the preexisting pending_ is less than that of sentinel_size. This means that the entire string that was to be searched through, which doesn't have the sentinel_ in it, can be placed within pending_ so that further chunks, if they exist can be added. The second case is where the growth factor is actually used. The pending_ only takes a portion of the substring which is pending_ = total.substr(total_size - sentinel_size + 1, sentinel_size - 1). An example would be a total size of 20 (meaning indexing from 0 to 19) and a sentinel_size of 8. As there isn't a sentinel_ within "total", the first 13 characters can be taken out, with only the last 7 being kept as per the sentinel_.size() - 1 rule. So, total.substr(13,7) allows a substring from index 13 of "total" to index 19, which won't result in any out of bounds errors, and also keeps it at O(1) space (constant space) allocated for this pending_ private variable. 


## What I would change differently
As I wasn't able to fully implement the test cases, I would spend more time on that to know the shortcomings of my program. I also started this project very late, which caused me to spend a lot of time in which I could have asked valuable questions to the TAs. However, there was a lot of reference material from the slides and the guide for sentinel_scanner which allowed me to create solutions that work to the extent that I've tested it. I also didn't have that much time to add "rigorous" testing, and I only implemented one example per test.
I would also have reviewed more on the rule of 5 before starting this because that would have saved me a lot of time. However, I have received a lot of knowledge on it and also on the edge cases that are needed to be implemented within the rule of 5 for this specific project, like the empty/starting conversation case.
