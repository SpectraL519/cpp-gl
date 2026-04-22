# Default to 'dev' if TAGS is not specified on the command line
TAGS ?= dev

.PHONY: docs serve-docs clean-docs clean

docs:
	@echo "==> Deploying MkDocs documentation locally via mike (Tags: $(TAGS))..."
	uv run mike deploy $(TAGS)
	@echo "==> Documentation deployed to local gh-pages branch."
# Example usage: `make docs TAGS="v2.0.0 latest --update-aliases"`

serve-docs:
	@echo "==> Serving versioned MkDocs documentation locally via mike..."
	uv run mike serve

clean-docs:
	@echo "==> Cleaning documentation build directories..."
	rm -rf site/
	rm -rf documentation/

clean: clean-docs
