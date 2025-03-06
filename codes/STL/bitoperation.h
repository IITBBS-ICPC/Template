// Iterate over submasks
for(int sub = mask; sub; sub = (sub - 1) & mask)
  cout << sub << "\n"; // Descending order

int cur = (1 << r) - 1, lowest_bit, ones;
while(cur < (1 << n)) {
  // Printing current binary number with r set bits
  for(int i = n - 1; i >= 0; i--)
    cout << ((cur >> i) & 1);
  cout << "\n";
  lowest_bit = cur & (-cur);
  ones = cur & ~(cur + lowest_bit);
  if(!r) break;
  else
    cur = cur + lowest_bit + (ones / lowest_bit / 2);
}
