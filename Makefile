# Default to 'dev' if TAGS is not specified on the command line
TAGS ?= dev

.PHONY: docs build-docs serve-docs clean-docs clean

# Target for CI Pull Requests: Just builds to verify everything works (no deployment)
build-docs: clean-docs
	@echo "==> Building MkDocs documentation..."
	doxygen Doxyfile
	uv run python docs/scripts/gen_concept_docs.py --config docs/config/concepts.json --xml documentation/xml --out docs/cpp-gl
	uv run mkdocs build --strict

# Example usage: `make docs TAGS="v2.0.0 latest --update-aliases --push"`
docs: clean-docs
	@echo "==> Deploying MkDocs documentation (Tags: $(TAGS))..."
	doxygen Doxyfile
	uv run python docs/scripts/gen_concept_docs.py --config docs/config/concepts.json --xml documentation/xml --out docs/cpp-gl
	uv run mike deploy $(TAGS)
	@echo "==> Documentation deployed to local gh-pages branch."

serve-docs: docs
	@echo "==> Serving versioned MkDocs documentation..."
	uv run mike serve

clean-docs:
	@echo "==> Cleaning documentation build directories..."
	rm -rf docs/cpp-gl/
	rm -rf site/
	rm -rf documentation/

clean: clean-docs
