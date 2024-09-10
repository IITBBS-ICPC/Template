// Note: Bitset is not a dynamic array, the size must be known at compile time

// Initilisation
bitset<10> b1; // 10 bits initialised to 0
bitset<10> b1(12) // 10 bits initialised to 0000001100
bitset<10> b1("101010"); // 10 bits initialised to 0000101010

// All operations are 0 - indexed based
// Note that the bits are stored in reverse order
// i.e. b[0] is the rightmost bit

// Set
b1.set(); // Set all bits to 1
b1.set(0); // Set b[0] to 1

// Reset
b1.reset(); // Set all bits to 0
b1.reset(0); // Set b[0] to 0

// Flip
b1.flip(); // Flip all bits
b1.flip(0); // Flip b[0]

// Count 1's
b1.count(); // Count number of 1s

// Get value
b1.test(0); //  Get b[0] value

// Bitwise operations
b1 & b2; b1 | b2; b1 ^ b2; ~b1;

// Shift operations
b1 << 1; // Left shift
b1 >> 1; // Right shift

// Comparison
b1 == b2; // Check if equal

// To string
b1.to_string(); // Convert to string

// Any, All, None
b1.any(); // Check if any bit is set
b1.all(); // Check if all bits are set
b1.none(); // Check if no bits are set

// Find first, Find next
// Note that it begins with _ then F (capital) and then first/next
b1._Find_first(); // Find first set bit
b1._Find_next(0); // Find next set bit after b[0] (not including)



