gcc -o asdf lab2_task4.c
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./asdf 2> without_g.txt

gcc -o asdf lab2_task4.c -g
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./asdf 2> with_g.txt

cat without_g.txt | cut -f2- -d' ' > without_g.txt
cat with_g.txt | cut -f2- -d' ' > with_g.txt

diff without_g.txt with_g.txt