
HEADERS = PolarCodes/bhattacharyya_files.h \
		PolarCodes/channels.h \
		PolarCodes/double_ei.h \
		PolarCodes/pc_construction.h \
		PolarCodes/pc_decode_internal.h \
		PolarCodes/pc_decode.h \
		PolarCodes/pc_encode.h \
		PolarCodes/polar_codes.h 

OBJECTS = PolarCodes/bhattacharyya_files.o \
 	PolarCodes/channels.o \
 	PolarCodes/double_ei.o \
 	PolarCodes/pc_construction.o \
 	PolarCodes/pc_decode_internal.o \
 	PolarCodes/pc_decode.o \
 	PolarCodes/pc_encode.o \
 	PolarCodes/polar_codes.o

MAIN_FILES = PC_pc_construction/main.o PC_pc_simulate/main.o

EXECUTABLE = pc_construction pc_simulate


pc_construction: $(OBJECTS) PC_pc_construction/main.o
	gcc $(OBJECTS) PC_pc_construction/main.o -lm -o $@

pc_simulate: $(OBJECTS) PC_pc_simulate/main.o
	gcc $(OBJECTS) PC_pc_simulate/main.o -lm -o $@




default: $(EXECUTABLE)

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

clean:
	-rm -f $(OBJECTS) $(MAIN_FILES)
	-rm -f $(EXECUTABLE)

