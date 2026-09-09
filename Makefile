# ============================================
# Build Configuration
# ============================================
BIN_DIR := output
SRC_DIR := examples
CC := cc
CFLAGS := -Wall -Wextra -I. -g

# Targets to build and test
TARGETS := array ring_buffer deque string_view

# ============================================
# Build Rules
# ============================================
all: $(addprefix $(BIN_DIR)/, $(TARGETS))

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# ============================================
# Help
# ============================================
.PHONY: help
help:
	@echo "=========================================="
	@echo "  Available targets"
	@echo "=========================================="
	@echo ""
	@echo "  make              Build all targets (array, ring_buffer, deque, string_view)"
	@echo "  make memcheck     Run full Valgrind memory check on all targets"
	@echo "  make quick        Run quick Valgrind summary check on all targets"
	@echo "  make clean        Remove build artifacts and logs"
	@echo "  make help         Show this help message"
	@echo ""

# ============================================
# Valgrind Check (automated)
# ============================================
.PHONY: check memcheck
check: memcheck

memcheck: all
	@echo "=========================================="
	@echo "Running Valgrind memory checks..."
	@echo "=========================================="
	@if ! command -v valgrind > /dev/null 2>&1; then \
		echo ""; \
		echo "❌ Error: valgrind not found. Please install it:"; \
		echo "   sudo apt install valgrind      # Ubuntu/Debian"; \
		echo "   sudo dnf install valgrind      # Fedora"; \
		echo "   sudo pacman -S valgrind        # Arch"; \
		echo ""; \
		exit 1; \
	fi
	@$(MAKE) _check_all

# Internal rule: check all targets
_check_all:
	@PASSED=0; FAILED=0; \
	for target in $(TARGETS); do \
		echo ""; \
		echo "[$$target]"; \
		valgrind --leak-check=full \
		         --show-leak-kinds=all \
		         --error-exitcode=1 \
		         --quiet \
		         ./$(BIN_DIR)/$$target > /tmp/valgrind_$$target.log 2>&1; \
		if [ $$? -eq 0 ]; then \
			echo "  ✅ PASSED: No memory errors detected."; \
			PASSED=$$((PASSED + 1)); \
		else \
			echo "  ❌ FAILED: Memory errors found."; \
			cat /tmp/valgrind_$$target.log; \
			FAILED=$$((FAILED + 1)); \
		fi; \
	done; \
	echo ""; \
	echo "=========================================="; \
	echo "Summary: $$PASSED passed, $$FAILED failed"; \
	echo "=========================================="; \
	if [ $$FAILED -gt 0 ]; then exit 1; fi

# ============================================
# Quick check (summary only)
# ============================================
.PHONY: quick
quick: all
	@echo "=========================================="
	@echo "Quick Valgrind check (summary)..."
	@echo "=========================================="
	@if ! command -v valgrind > /dev/null 2>&1; then \
		echo ""; \
		echo "❌ Error: valgrind not found. Please install it:"; \
		echo "   sudo apt install valgrind      # Ubuntu/Debian"; \
		echo "   sudo dnf install valgrind      # Fedora"; \
		echo "   sudo pacman -S valgrind        # Arch"; \
		echo ""; \
		exit 1; \
	fi
	@for target in $(TARGETS); do \
		echo -n "[$$target] "; \
		valgrind --leak-check=full \
		         --error-exitcode=1 \
		         --quiet \
		         ./$(BIN_DIR)/$$target 2>&1 | \
			grep -E "ERROR SUMMARY|definitely lost|indirectly lost" | \
			head -3; \
	done

# ============================================
# Clean
# ============================================
clean:
	rm -rf $(BIN_DIR)
	rm -f /tmp/valgrind_*.log

.PHONY: all clean
