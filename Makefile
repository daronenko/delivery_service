CMAKE_COMMON_FLAGS ?= -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_DEBUG_FLAGS ?= -DUSERVER_SANITIZE='addr ub'
CMAKE_RELEASE_FLAGS ?=
NPROCS ?= $(shell nproc)

CLANG_TIDY ?= clang-tidy
CLANG_FORMAT ?= clang-format

AUTOPEP8 ?= autopep8
FLAKE8 ?= flake8

DOCKER_COMPOSE ?= docker-compose

# NOTE: use Makefile.local to override the options defined above.
-include Makefile.local

CMAKE_DEBUG_FLAGS += -DCMAKE_BUILD_TYPE=Debug $(CMAKE_COMMON_FLAGS)
CMAKE_RELEASE_FLAGS += -DCMAKE_BUILD_TYPE=Release $(CMAKE_COMMON_FLAGS)

.PHONY: all
all: test-debug test-release

# Run cmake
.PHONY: cmake-debug
cmake-debug:
	git submodule update --init
	cmake -B build_debug $(CMAKE_DEBUG_FLAGS)

.PHONY: cmake-release
cmake-release:
	git submodule update --init
	cmake -B build_release $(CMAKE_RELEASE_FLAGS)

build_debug/CMakeCache.txt: cmake-debug
build_release/CMakeCache.txt: cmake-release

# Build using cmake
.PHONY: build-debug build-release
build-debug build-release: build-%: build_%/CMakeCache.txt
	cmake --build build_$* -j $(NPROCS) --target delivery_service

# Test
.PHONY: test-debug test-release
test-debug test-release: test-%:
	# cmake --build build_$* -j $(NPROCS) --target delivery_service_unittest
	# cmake --build build_$* -j $(NPROCS) --target delivery_service_benchmark
	cd build_$* && ((test -t 1 && GTEST_COLOR=1 PYTEST_ADDOPTS="--color=yes" ctest -V) || ctest -V)
	pep8 tests

# Start the service (via testsuite service runner)
.PHONY: service-start-debug service-start-release
service-start-debug service-start-release: service-start-%: build-%
	cmake --build build_$* -v --target start-delivery_service

# Cleanup data
.PHONY: clean-debug clean-release
clean-debug clean-release: clean-%:
	cmake --build build_$* --target clean

.PHONY: dist-clean
dist-clean:
	rm -rf build_*
	rm -rf tests/__pycache__/
	rm -rf tests/.pytest_cache/

.PHONY: install-debug
install-debug:
	cmake --install build_debug -v --component delivery_service

.PHONY: install-release
install-release: build-release
	cmake --install build_release -v --component delivery_service

.PHONY: install
install: install-release

# Format the sources
.PHONY: format
format:
	find src -name '*pp' -type f | xargs $(CLANG_FORMAT) -i
	find tests -name '*.py' -type f | xargs $(AUTOPEP8) -i

.PHONY: lint
lint:
	find src -name '*pp' -type f | xargs $(CLANG_TIDY) -p build_$*
	find tests -name '*.py' -type f | xargs $(FLAKE8)

# Internal hidden targets that are used only in docker environment
--in-docker-start-debug --in-docker-start-release: --in-docker-start-%: install-%
	psql 'postgresql://user:password@postgres:5432/delivery_service_db_1' -f ./postgresql/schemas/db_1.sql
	psql 'postgresql://user:password@postgres:5432/delivery_service_db_1' -f ./postgresql/data/initial_data.sql
	/home/user/.local/bin/delivery_service \
		--config /home/user/.local/etc/delivery_service/static_config.yaml \
		--config_vars /home/user/.local/etc/delivery_service/config_vars.docker.yaml

# Run service in docker environment
.PHONY: docker-start-service-debug docker-start-service-release
docker-start-service-debug docker-start-service-release: docker-start-service-%:
	$(DOCKER_COMPOSE) run -p 8080:8080 --rm delivery_service_container make -- --in-docker-start-$*

# Start targets makefile in docker environment
.PHONY: docker-cmake-debug docker-build-debug docker-test-debug docker-clean-debug docker-install-debug docker-cmake-release docker-build-release docker-test-release docker-clean-release docker-install-release
docker-cmake-debug docker-build-debug docker-test-debug docker-clean-debug docker-install-debug docker-cmake-release docker-build-release docker-test-release docker-clean-release docker-install-release: docker-%:
	$(DOCKER_COMPOSE) run --rm delivery_service_container make $*

# Stop docker container and remove PG data
.PHONY: docker-clean-data
docker-clean-data:
	$(DOCKER_COMPOSE) down -v
	rm -rf ./.pgdata
