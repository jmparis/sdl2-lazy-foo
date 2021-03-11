TOPTARGETS	=	clean

NODIRS		?=
SUBDIRS		:=	$(sort $(filter-out $(addsuffix /,$(NODIRS)),$(wildcard ./*/)))

.PHONY:		$(TOPTARGETS) $(SUBDIRS)

$(TOPTARGETS):	$(SUBDIRS)

$(SUBDIRS):
	@printf "\r"
	@printf "\033[1;34m[\033[1;35m$@\033[1;34m]\033[0m\n"
	@$(MAKE) --no-print-directory --directory='$@' $(MAKECMDGOALS)
