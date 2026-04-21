.PHONY: doxy clean-doxy clean

doxy:
	@echo "==> Building Doxygen documentation..."
	doxygen Doxyfile
	uv run python scripts/postprocess_doxyhtml.py documentation/ --img-rules docs/style/img_style_rules.json
	@echo "==> Doxygen build complete."

clean-doxy:
	@echo "==> Cleaning Doxygen build directory..."
	rm -rf documentation/

clean: clean-doxy
