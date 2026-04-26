# Default to 'dev' if TAGS is not specified on the command line
TAGS ?= dev

.PHONY: docs serve-docs clean-docs clean doxy clean-doxy

# Example usage: `make docs TAGS="v2.0.0 latest --update-aliases"`
docs: clean-docs
	@echo "==> Deploying MkDocs documentation locally via mike (Tags: $(TAGS))..."
	uv run mike deploy $(TAGS)
	@echo "==> Documentation deployed to local gh-pages branch."

serve-docs: docs
	@echo "==> Serving versioned MkDocs documentation locally via mike..."
	uv run mike serve

docs-concepts: clean-docs
	@echo "==> Deploying MkDocs documentation locally via mike (Tags: $(TAGS))..."
	doxygen Doxyfile
	uv run python scripts/gen_concept_docs.py --config docs/config/concepts.json --xml documentation/xml --out docs/cpp-gl
	uv run mike deploy $(TAGS)
	@echo "==> Documentation deployed to local gh-pages branch."

serve-docs-concepts: docs-concepts
	@echo "==> Serving versioned MkDocs documentation locally via mike..."
	uv run mike serve

clean-docs:
	@echo "==> Cleaning documentation build directories..."
	rm -rf docs/cpp-gl/
	rm -rf site/
	rm -rf documentation/

clean: clean-docs clean-doxy

doxy:
	@echo "==> Building Doxygen documentation..."
	doxygen Doxyfile
	uv run python scripts/postprocess_doxyhtml.py documentation/ --img-rules docs/style/img_style_rules.json
	@echo "==> Doxygen build complete."

clean-doxy:
	@echo "==> Cleaning Doxygen build directory..."
	rm -rf documentation/html/
