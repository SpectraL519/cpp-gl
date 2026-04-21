.PHONY: docs serve-docs clean-docs clean

docs:
	@echo "==> Building MkDocs documentation..."
	uv run mkdocs build
	@echo "==> Documentation build complete. Output is in site/"

serve-docs:
	@echo "==> Serving MkDocs documentation locally..."
	uv run mkdocs serve

clean-docs:
	@echo "==> Cleaning documentation build directories..."
	rm -rf site/
	rm -rf xml/

clean: clean-docs
