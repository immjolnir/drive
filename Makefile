
WORKING_DIR :=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

default:
	echo "NO such target"

.PHONY: cleanup
cleanup:
	rm -rf build
	mkdir build

algos:
	cmake -B build -S . -D ENABLE_ALGOS=ON -D ENABLE_SANITIZER=ON
	make -C build

json:
	cmake -B build -S . -D ENABLE_JSON=ON -D ENABLE_SANITIZER=ON
	make -C build

arrow:
	cmake -B build -S . -D ENABLE_ARROW=ON
	make -C build

logging:
	cmake -B build -S . -D ENABLE_LOGGING=ON
	make -C build

boost:
	cmake -B build -S . -D ENABLE_BOOST=ON
	make -C build

eigen:
	cmake -B build -S . -D ENABLE_EIGEN=ON
	make -C build

mytest:
	cmake -B build -S . -D ENABLE_MYTEST=ON
	make -C build

3d:
	cmake -B build -S . -D ENABLE_3D=ON
	make -C build
	make -C build test

pb:
	cmake -B build -S . -D ENABLE_PROTOBUFF=ON -D DEVELOPMENT_BUILD=ON 
	make -C build
	make -C build test

opencv:
	cmake -B build -S . -D ENABLE_MODULE_LEARN_OPENCV=ON -D BUILD_TESTING=ON -D DEVELOPMENT_BUILD=ON 
	make -C build
	make -C build test

basis: 
	cmake -B build -S . -D ENABLE_MODULE_BASIS=ON -D BUILD_TESTING=ON -D DEVELOPMENT_BUILD=ON  -D ENABLE_SANITIZER=ON
	make -C build
	make -C build test

test:
	cmake -B build -S . \
		-DARCHETYPE_DEVELOPMENT_BUILD=ON \
		-DARCHETYPE_BUILD_TESTING=ON \
		-DARCHETYPE_ENABLE_COVERAGE=ON
	make -C build
	make -C build test
	# ctest --rerun-failed --output-on-failure

.PHONY: coverage
coverage: test
	make -C build coverage

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
