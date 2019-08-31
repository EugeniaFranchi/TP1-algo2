EXEC = trabajo #NOMBRE EJECUTABLE
#Si pongo objfiles = nada y escribo todos los C en CFILES_SUELTOS -> Solo linkeo, no compilo
OBJFILES = strutil.o lista.o pila.o pago.o usuario.o #OBJFILES ESPERADOS
CFILES_SUELTOS = wachencoin.c #DE QUE .C DEPENDE QUE NO TENGAN RESPECTIVOS OBJS

CC = gcc
CFLAGS = -g -std=c99 -Wall
CFLAGS += -Wconversion -Wno-sign-conversion -Werror -Wbad-function-cast -Wshadow
CFLAGS += -Wpointer-arith -Wunreachable-code -Wformat=2
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes
GDB_FLAGS = -quiet  -ex 'set pagination off' -ex 'set confirm off'

#Compila y linkea
all: $(EXEC)

#Corre	
run: all
	./$(EXEC) archivo.txt

#Valgrindea
valgrind: all
	valgrind $(VFLAGS) ./$(EXEC) archivo.txt

#Corre gdb y printea primeras 30 lineas
gdb: all
	gdb $(GDB_FLAGS) -ex '1,30' -'start' ./$(EXEC) 

#Corre gdb con interfaz
gdb_tui: all
	gdb $(GDB_FLAGS)  -tui ./$(EXEC) 

#Limpia los objetos y el ejecutable
clean:
	rm -f *.o $(EXEC)

#Tiempo de ejecuccion
runtime: all
	time ./$(EXEC)


#Compilador	
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
	
#Linkeador
$(EXEC): $(OBJFILES) $(CFILES_SUELTOS)
	$(CC) $(CFLAGS) $(OBJFILES) $(CFILES_SUELTOS) -o $(EXEC)

