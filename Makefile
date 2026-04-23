# Default to 'dev' if TAGS is not specified on the command line
TAGS ?= dev

.PHONY: docs serve-docs clean-docs clean

# Example usage: `make docs TAGS="v2.0.0 latest --update-aliases"`
docs:
	@echo "==> Deploying MkDocs documentation locally via mike (Tags: $(TAGS))..."
	doxygen Doxyfile
	uv run python scripts/gen_concept_docs.py --config docs/concepts_cfg.json --xml documentation/xml --out docs/cpp-gl
	uv run mike deploy $(TAGS)
	@echo "==> Documentation deployed to local gh-pages branch."

serve-docs: docs
	@echo "==> Serving versioned MkDocs documentation locally via mike..."
	uv run mike serve

clean-docs:
	@echo "==> Cleaning documentation build directories..."
	rm -rf site/
	rm -rf documentation/

clean: clean-docs
