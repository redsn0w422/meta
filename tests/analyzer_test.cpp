/**
 * @file analyzer_test.cpp
 * @author Sean Massung
 */

#include "analyzers/all.h"
#include "analyzers/token_stream.h"
#include "bandit/bandit.h"
#include "corpus/document.h"
#include "create_config.h"
#include "io/filesystem.h"
#include "util/shim.h"

using namespace bandit;
using namespace meta;

namespace {

std::unique_ptr<analyzers::token_stream> make_filter() {
    using namespace analyzers;
    auto line_cfg = tests::create_config("line");
    return analyzers::default_filter_chain(*line_cfg);
}

template <class Analyzer>
void check_analyzer_expected(Analyzer& ana, corpus::document doc,
                             uint64_t num_unique, uint64_t length) {
    auto counts = ana.analyze(doc);
    AssertThat(counts.size(), Equals(num_unique));

    auto total = std::accumulate(
        counts.begin(), counts.end(), uint64_t{0},
        [](uint64_t acc,
           const typename Analyzer::feature_map::value_type& count) {
            return acc + count.second;
        });

    AssertThat(total, Equals(length));
    AssertThat(doc.id(), Equals(47ul));
}
}

go_bandit([]() {

    describe("[analyzers]: string content", []() {

        corpus::document doc{doc_id{47}};
        // "one" is a stopword
        std::string content = "one one two two two three four one five";
        doc.content(content);

        it("should tokenize unigrams from a string", [&]() {
            analyzers::ngram_word_analyzer<uint64_t> tok{1, make_filter()};
            check_analyzer_expected(tok, doc, 6, 8);
        });

        it("should tokenize bigrams from a string", [&]() {
            analyzers::ngram_word_analyzer<uint64_t> tok{2, make_filter()};
            check_analyzer_expected(tok, doc, 6, 7);
        });

        it("should tokenize trigrams from a string", [&]() {
            analyzers::ngram_word_analyzer<uint64_t> tok{3, make_filter()};
            check_analyzer_expected(tok, doc, 6, 6);
        });
    });

    describe("[analyzers]: file content", [&]() {

        corpus::document doc{doc_id{47}};
        doc.content(filesystem::file_text("../data/sample-document.txt"));

        it("should tokenize unigrams from a file", [&]() {
            analyzers::ngram_word_analyzer<uint64_t> tok{1, make_filter()};
            check_analyzer_expected(tok, doc, 93, 168);
        });

        it("should tokenize bigrams from a file", [&]() {
            analyzers::ngram_word_analyzer<uint64_t> tok{2, make_filter()};
            check_analyzer_expected(tok, doc, 140, 167);
        });

        it("should tokenize trigrams from a file", [&]() {
            analyzers::ngram_word_analyzer<uint64_t> tok{3, make_filter()};
            check_analyzer_expected(tok, doc, 159, 166);
        });
    });
});
