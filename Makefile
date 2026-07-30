# Default to 'dev' if TAGS is not specified on the command line
TAGS ?= dev

# Detect if running in GitHub Actions (or any standard CI)
ifeq ($(CI),true)
    QUIET    :=
else
    QUIET    := > /dev/null 2>&1
endif

.PHONY: docs build-docs serve-docs clean-docs clean sync-gh-pages

# Target for CI Pull Requests: Just builds to verify everything works (no deployment)
build-docs: clean-docs
	@echo "==> Building MkDocs documentation..."
	doxygen Doxyfile
	uv run python docs/scripts/gen_concept_docs.py --config docs/config/concepts.json --xml documentation/xml --out docs/cpp-gl
	uv run mkdocs build --strict

# Example usage:
# - Building the docs for a single tag locally: `make docs TAGS="v<num>-dev"`
# - Building and deploying the docs: `make docs TAGS="v<num> latest --update-aliases --push"`
docs: clean-docs
	@echo "==> Deploying MkDocs documentation (Tags: $(TAGS))..."
	@doxygen Doxyfile $(QUIET)
	@uv run python docs/scripts/gen_concept_docs.py --config docs/config/concepts.json --xml documentation/xml --out docs/cpp-gl $(QUIET)
	@uv run mike deploy $(TAGS) $(QUIET)
	@echo "==> Documentation deployed to local gh-pages branch."

serve-docs: docs
	@echo "==> Serving versioned MkDocs documentation..."
	uv run mike serve

clean-docs:
	@echo "==> Cleaning documentation build directories..."
	@rm -rf docs/cpp-gl/
	@rm -rf site/
	@rm -rf documentation/
	@echo "==> Documentation build cleaning complete!"

clean: clean-docs

sync-gh-pages:
	@echo "==> Fetching latest gh-pages from remote..."
	@git fetch origin gh-pages
	@echo "==> Force-updating local gh-pages to match origin..."
	@git branch -f gh-pages origin/gh-pages
	@echo "==> gh-pages sync complete!"
