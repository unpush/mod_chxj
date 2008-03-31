.SUFFIXES:
.SUFFIXES: .sdoc .html

TARGET=modchxj.html

all: $(TARGET)

.sdoc.html:
	sed "s/@@DATE@@/`LANG=C date`/" $< > /tmp/modchxj.sdoctmp
	sdoc -toc:true  /tmp/modchxj.sdoctmp

clean:
	rm -f $(TARGET)

