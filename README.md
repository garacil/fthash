Copyright (c) 2019, Germán Luis Aracil Boned

Fast hash table (fthash)

It's an implementation of hash table with very important features to run very fast:

- Macros: All code are implemented with macros. No external functions and jumps.
- Expanded Index: The table have an index created when init with custom size in bits.
- Buckets: No buckets limit and collision's control implemented.
- Flag control: You have a char to store any flag about data to check when read data. 
- Hardware: It's usable with full servers with important ammount of memory and small devices like ESP32.

Defined macros to use:

-------------------------------------
#define HASH_TABLE_INIT(ht, is, out)
Use first to init table where:

ht: Ptr to variable HASHTABLE_T

is: Index size in bits. From 1 to 32. Usual use can be 8 to 24 bits. 
    It depend of ammount of data to store. Try with diferent sizes before.
    
out: and int variable. when all is okey, exit with 0.

-------------------------------------
#define HASH_TABLE_ADD(ht, k, d, f, out)
Use to add record to table where:

ht: Ptr to variable HASHTABLE_T

k: Ptr to char key

d: Ptr to data (void*) Can be char, ptr to struct...

f: Flag type char. To store any information about data. Free to use if you need it.

out: Zero when record was add

-------------------------------------
#define HASH_TABLE_GET(ht, k, d, f)
Use to recover data from table where:

ht: Ptr to variable HASHTABLE_T

k: Ptr to char key to search

d: Pointer to data

f: Flag of data

-------------------------------------
#define HASH_TABLE_ERA(ht, k, out)
Use to remove data from table where:

ht: Ptr to variable HASHTABLE_T

k: Ptr to char key to search and remove

out: If record was removed 1 else 0


To test it
----------

Compile test.c with gcc -O3 test.c -o test and run:

INITIAL DATA
Table structure size 48 bytes
Index 24 bits, size in memory 134217720 bytes
Buckets 0, size in memory 0 bytes
Total hash collisions 0
Seting up pointers to random data...
Inserting 1000000 of records...
(     0) INSERT Key 6cce35c487a58feab29032c2621f38d9 Value 840187
(100000) INSERT Key 17c8ef30ac7903f63b6549d3f5f2bc4c Value 148897
(200000) INSERT Key 789a2f5cfdf69c0dc5806b281a401578 Value 420991
(300000) INSERT Key 8dcf69c4beb75517d3b563a31438b1c3 Value 302532
(400000) INSERT Key 95d27693a5a67c67c2ebf4bfe3f8fc08 Value 498211
(500000) INSERT Key b803239cd6cb179729765783858dc587 Value 702006
(600000) INSERT Key cd5fbe4b36cc4f8280840396375969b3 Value 169289
(700000) INSERT Key 1347af69a0209a0e1774fbf57d7d4d56 Value 499362
(800000) INSERT Key 7e52c4fcbaf988c6105f05ceacb43b9a Value 337676
(900000) INSERT Key e08ab2e407380fcf4f6c83605f91085f Value 295103
INSERTED 1000000 records
Table structure size 48 bytes
Index 24 bits, size in memory 134217720 bytes
Buckets 1000000, size in memory 32000000 bytes
Total hash collisions 29452
Total time in seconds 0.677374, by record 0.677373999999 micro seconds
Get 1000000 random records...
(     0) READ Key 9adfd489eae898a694ef682623269d62 Value 694
(100000) READ Key 7acdd169dcf57c626dff503aaad18bdf Value 768984
(200000) READ Key f355c5b55a8f1e6b139dfad90ae317d0 Value 656831
(300000) READ Key 186af1f4a78a78fe2dfdd47237976328 Value 635864
(400000) READ Key 11f04227106fff2105e484e1e65092aa Value 628262
(500000) READ Key a670f074521e68545ce8d44e2ef77711 Value 652855
(600000) READ Key d6e7c3b1b29e35f2ff5d399be7575182 Value 21591
(700000) READ Key d3ac45f58ec9acbc8031822afbfeac67 Value 476885
(800000) READ Key 17c347ab854251229a323284fdb2bbfd Value 823266
(900000) READ Key 96005ce3741c45cbcc133144a185ff79 Value 117398
Get 1000000 of random records time in seconds 1.088843, by record 0.108884300000 micro seconds
ERASE DATA
Table structure size 48 bytes
Index 24 bits, size in memory 134217720 bytes
Buckets 0, size in memory 0 bytes
Total hash collisions 0

Enjoy!

