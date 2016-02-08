CC	= /opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j

all:	pookey.gb

%.o:	%.c
	$(CC) -c -o $@ $<

%.s:	%.c
	$(CC) -S -o $@ $<

%.o:	%.s
	$(CC) -c -o $@ $<

pookey.gb:	pookey.o game/physics.o game/gameplay.o
	$(CC) -o pookey.gb pookey.o game/physics.o  game/gameplay.o $<

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm
	rm -f game/*.o game/*.lst game/*.map *.gb *~ game/*.rel game/*.cdb game/*.ihx game/*.lnk game/*.sym game/*.asm

