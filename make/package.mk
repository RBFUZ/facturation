update:
	if [ -f update.sh ]; then \
	    bash update.sh; \
	else \
	    wget -q -O - https://storage.projectsforge.org/update/c/update.sh | bash; \
	fi

package: update
	bash make/package.sh
