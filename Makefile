
WORKING_DIR :=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

default:
	echo "NO such target"

.PHONY: cleanup
cleanup:
	rm -rf build
	mkdir build

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
