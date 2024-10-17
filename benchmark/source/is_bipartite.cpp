#include "clock.hpp"
#include "numeric_format.hpp"

#include <ap/argument_parser.hpp>

#include <gl/graph.hpp>
#include <gl/topologies.hpp>
#include <gl/algorithms.hpp>

#include <locale>
#include <numeric>
#include <fstream>

int main(int argc, char** argv) {
    ap::argument_parser parser;

    parser.add_positional_argument<gl::types::size_type>("n_vertices_a")
          .help("Number of vertices in the first vertex set");
    parser.add_positional_argument<gl::types::size_type>("n_vertices_b")
          .help("Number of vertices in the second vertex set");

    parser.add_optional_argument("o", "output")
          .help("The output file name");
    parser.add_optional_argument<std::size_t>("r", "repetitions")
          .default_value(1ull)
          .help("The number of experiment repetitions");

    parser.parse_args(argc, argv);

    const auto n_vertices_a = parser.value<gl::types::size_type>("n_vertices_a");
    const auto n_vertices_b = parser.value<gl::types::size_type>("n_vertices_b");

    const auto n_repetitions = parser.value<std::size_t>("repetitions");

    using traits_type = gl::list_graph_traits<gl::undirected_t>;
    using graph_type = gl::graph<traits_type>;

    const auto graph = gl::topology::biclique<graph_type>(n_vertices_a, n_vertices_b);

    std::locale loc(std::cout.getloc(), new thousend_sepparator());
    std::cout.imbue(loc);

    std::cout << "n_vertices: " << graph.n_vertices()
              << "\nn_edges: " << graph.n_unique_edges() << std::endl;

    std::vector<std::chrono::milliseconds> durations;
    durations.reserve(n_repetitions);

    for (std::size_t i = 0ull; i < n_repetitions; i++) {
        std::cout << std::format("Running: {}/{}", i + 1, n_repetitions) << std::endl;

        const auto [is_bipartite, duration] = MEASURE_TIME_WITH_RESULT(gl::algorithm::is_bipartite(graph));
        if (not is_bipartite) {
            std::cerr << "is_bipartite = false" << std::endl;
            std::exit(1);
        }

        durations.push_back(duration);
    }

    const auto total_time = std::accumulate(
        durations.begin(),
        durations.end(),
        std::chrono::milliseconds(0)
    );
    const auto avg_time = total_time.count() / n_repetitions;

    std::cout << "\navg_time: " << avg_time << "ms" << std::endl;

    if (parser.has_value("output")) {
        std::cout << "Saving results..." << std::endl;

        std::ofstream file;
        file.open(parser.value("output"), std::ios::app);
        file << std::format("{},{},{}\n", graph.n_vertices(), graph.n_unique_edges(), avg_time);

        std::cout << "Done!" << std::endl;
    }
}
