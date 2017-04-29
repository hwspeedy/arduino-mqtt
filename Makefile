all: fmt

fmt:
	clang-format -i src/*.cpp src/*.h -style="{BasedOnStyle: Google, ColumnLimit: 120}"
	clang-format -i examples/**/*.ino -style="{BasedOnStyle: Google, ColumnLimit: 120}"

update:
	rm -rf ./lwmqtt
	git clone https://github.com/256dpi/lwmqtt.git ./lwmqtt
	mkdir -p ./src/lwmqtt
	cp -r ./lwmqtt/src/*.c ./src/lwmqtt/
	cp -r ./lwmqtt/src/*.h ./src/lwmqtt/
	cp -r ./lwmqtt/include/*.h ./src/lwmqtt/
	rm -rf ./lwmqtt
	sed -i '' "s/<lwmqtt.h>/\"lwmqtt.h\"/g" ./src/lwmqtt/*
