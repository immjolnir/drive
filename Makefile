
WORKING_DIR :=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

NUM_JOBS := $(shell nproc --all)

CMAKE_GENERATOR_NAME := $(shell echo ${CMAKE_GENERATOR_NAME})
ifeq (, ${CMAKE_GENERATOR_NAME})
  CMAKE_GENERATOR_NAME := Unix Makefiles
endif

ifeq (Ninja, ${CMAKE_GENERATOR_NAME})
  BUILD_COMMAND := ninja -j $(NUM_JOBS)
else
  BUILD_COMMAND := $(MAKE) --jobs $(NUM_JOBS) --no-print-directory
endif

# TIME_COMMAND =/usr/bin/time --format "buildtime: real=%e user=%U sys=%S [ %C ]"
TIME_COMMAND := "time" # not found the time command

default:
	echo "NO such target"

.PHONY: cleanup
cleanup:
	rm -rf build
	mkdir build

asm:
	cmake -B build -S . -D ENABLE_ASSEMBLER=ON
	$(BUILD_COMMAND) -C build

pattern:
	cmake -B build -S . -D ENABLE_DESIGN_PATTERN=ON
	$(BUILD_COMMAND) -C build
lc:
	cmake -B build -S . -D ENABLE_LEETCODE=ON
	$(BUILD_COMMAND) -C build

algos:
	cmake -B build -S . -D ENABLE_ALGOS=ON -D ARCHETYPE_DEVELOPMENT_BUILD=ON
	$(BUILD_COMMAND) -C build

json:
	cmake -B build -S . -D ENABLE_JSON=ON
	$(BUILD_COMMAND) -C build

arrow:
	cmake -B build -S . -D ENABLE_ARROW=ON
	$(BUILD_COMMAND) -C build

logging:
	cmake -B build -S . -D ENABLE_LOGGING=ON
	$(BUILD_COMMAND) -C build

boost:
	cmake -B build -S . -D ENABLE_BOOST=ON
	$(BUILD_COMMAND) -C build

eigen:
	cmake -B build -S . -D ENABLE_EIGEN=ON
	$(BUILD_COMMAND) -C build

mytest:
	cmake -B build -S . -D ENABLE_MYTEST=ON
	$(BUILD_COMMAND) -C build

mm:
	cmake -B build -S . -D ENABLE_MM=ON
	$(BUILD_COMMAND) -C build
	$(BUILD_COMMAND) -C build test

3d:
	cmake -B build -S . -D ENABLE_3D=ON
	$(BUILD_COMMAND) -C build
	$(BUILD_COMMAND) -C build test

ros:
	cmake -B build -S . -D ENABLE_ROS=ON
	$(BUILD_COMMAND) -C build
	$(BUILD_COMMAND) -C build test

pb:
	cmake -B build -S . -D ENABLE_PROTOBUFF=ON -D DEVELOPMENT_BUILD=ON 
	$(BUILD_COMMAND) -C build
	$(BUILD_COMMAND) -C build test

gtest:
	cmake -B build -S . -D ENABLE_GTEST=ON -D DEVELOPMENT_BUILD=ON
	$(BUILD_COMMAND) -C build
	$(BUILD_COMMAND) -C build test

opencv:
	cmake -B build -S . -D ENABLE_MODULE_LEARN_OPENCV=ON -D BUILD_TESTING=ON -D DEVELOPMENT_BUILD=ON 
	$(BUILD_COMMAND) -C build
	$(BUILD_COMMAND) -C build test

basis: 
	cmake -B build -S . -D ENABLE_MODULE_BASIS=ON -D BUILD_TESTING=ON -D DEVELOPMENT_BUILD=ON
	$(BUILD_COMMAND) -C build
	$(BUILD_COMMAND) -C build test

test:
	cmake -B build -S . \
		-DARCHETYPE_DEVELOPMENT_BUILD=ON \
		-DARCHETYPE_BUILD_TESTING=ON \
		-DARCHETYPE_ENABLE_COVERAGE=ON
	$(BUILD_COMMAND) -C build
	$(BUILD_COMMAND) -C build test
	# ctest --rerun-failed --output-on-failure

.PHONY: coverage
coverage: test
	$(BUILD_COMMAND) -C build coverage

.PHONY: docs
docs:
	cmake -Bbuild -DENABLE_DOXYGEN=ON
	cmake --build build --config Release
	cmake --build build --target doxygen-docs

.PHONY: examples
examples:
	cmake -Bbuild -D ARCHETYPE_DEVELOPMENT_BUILD=ON -D BUILD_EXAMPLES=ON -D ARCHETYPE_BUILD_TESTING=ON
	cmake --build build --config Release

.PHONY: docker
docker:
	export export DISPLAY=:0

	docker run --ipc=host --network host \
		-i -t --privileged \
		-v $(WORKING_DIR):/iwork \
		-v /work/data/Visual-Tracker-Benchmark/TB-50/:/work/data/Visual-Tracker-Benchmark/TB-50/ \
		-v /tmp/.X11-unix:/tmp/.X11-unix \
		mjolnir/opencv
