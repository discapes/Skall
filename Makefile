NAME = Skall.exe
.DEFAULT_GOAL := $(NAME)



CXXFLAGS = -std=c++20 -g -Wall -Wextra #-Wfatal-errors
LDLIBS = -lglad -lopengl32 -lglfw3 -lgdi32
LDFLAGS = -Ldep
INCLUDE = -Idep/include



rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

cpp_objects = $(patsubst src/%.cpp, obj/%.o, $(call rwildcard,src, *.cpp))
c_objects = $(patsubst src/%.c, obj/%.o, $(call rwildcard,src, *.c))
cpp_headers = $(call rwildcard,src, *.hpp)
c_headers = $(call rwildcard,src, *.h)
dirs = $(sort $(dir $(cpp_objects)) $(dir $(c_objects)))

$(cpp_objects): $(cpp_headers) $(c_headers)
$(c_objects): $(c_headers)
$(dirs) : ; mkdir -p $(dirs)

$(NAME): $(dirs) $(c_objects) $(cpp_objects) ; $(CXX) $(cpp_objects) $(c_objects) $(LDFLAGS) $(LDLIBS) -o $(NAME) 
obj/%.o : src/%.cpp ; $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $< $(INCLUDE)
obj/%.o : src/%.c ; $(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< $(INCLUDE)

.PHONY : clean ; 
clean : ; -rm -rf obj $(NAME)
