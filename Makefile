
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
	cmake -Bbuild -DARCHETYPE_DEVELOPMENT_BUILD=ON -DBUILD_EXAMPLES=ON
	cmake --build build --config Release
