SOURCE=main.elm
OUTPUT=main.js

.PHONY: clean cleanall run test debug

all: $(OUTPUT)

$(OUTPUT): $(SOURCE)
	elm-make $(SOURCE) --output=$(OUTPUT)

proper: clean cleanall

re: proper all

redo: proper debug

clean:
	rm -f $(OUTPUT)
