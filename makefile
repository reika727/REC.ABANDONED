SRCDIR =src
OBJDIR =obj
DEPDIR =dep
TRGDIR =bin
TESTDIR=test
SRCS   =$(wildcard $(SRCDIR)/*.cpp)
OBJS   =$(addprefix $(OBJDIR)/,$(notdir $(SRCS:.cpp=.o)))
DEPS   =$(addprefix $(DEPDIR)/,$(notdir $(SRCS:.cpp=.d)))
TESTSRC=$(TESTDIR)/test.c
TARGET =$(TRGDIR)/rec.out

.PHONY: all
all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	@mkdir -p $(TRGDIR)
	$(CXX) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@mkdir -p $(DEPDIR)
	$(CXX) $< -c -MMD -MP -o $@ -MF $(DEPDIR)/$(notdir $*).d

.PHONY: test
test: $(TARGET)
	./$(TARGET) $(TESTSRC) $(TESTDIR)/test.s
	$(CXX) $(TESTDIR)/test.s -o $(TESTDIR)/test.out
	./$(TESTDIR)/test.out
	@echo "succeeded"

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(DEPDIR) $(TRGDIR)
