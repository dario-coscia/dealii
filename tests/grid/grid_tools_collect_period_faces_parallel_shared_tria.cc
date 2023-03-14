// ---------------------------------------------------------------------
//
// Copyright (C) 2001 - 2023 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------

#include <deal.II/base/exceptions.h>
#include <deal.II/base/mpi.h>
#include <deal.II/base/point.h>
#include <deal.II/distributed/shared_tria.h>
#include <deal.II/distributed/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/grid/tria.h>

#include <vector>
using namespace dealii;
template <int dim>
void test(MPI_Comm comm) {
  deallog << "dim = " << dim << std::endl;
  parallel::shared::Triangulation<dim> tria(comm);

  std::vector<typename GridTools::PeriodicFacePair<
      typename parallel::shared::Triangulation<dim>::cell_iterator>>
      matched_pairs;

  unsigned int num_refinements = 1 << 4;

  if constexpr (dim == 1) {
    Point<dim> p1{0.};
    Point<dim> p2{1.};
    std::vector<unsigned int> repitions{num_refinements};
    GridGenerator::subdivided_hyper_rectangle(tria, repitions, p1, p2, true);

    // Collect periodic faces in the x-direction
    GridTools::collect_periodic_faces(tria, 0, 1, 0, matched_pairs);
    // Check the size of the matched_pairs vector
    if (matched_pairs.size() != 1) {
      // throw error
    }
  } else if constexpr (dim == 2) {
    Point<dim> p1{0., 0.};
    Point<dim> p2{1., 1.};
    std::vector<unsigned int> repitions{num_refinements, num_refinements};
    GridGenerator::subdivided_hyper_rectangle(tria, repitions, p1, p2, true);

    GridTools::collect_periodic_faces(tria, 0, 1, 0, matched_pairs);
    if (matched_pairs.size() != 16) {
    }
  } else if constexpr (dim == 3) {
    Point<dim> p1{0., 0., 0.};
    Point<dim> p2{1., 1., 1.};
    std::vector<unsigned int> repitions{num_refinements, num_refinements,
                                        num_refinements};
    GridGenerator::subdivided_hyper_rectangle(tria, repitions, p1, p2, true);

    GridTools::collect_periodic_faces(tria, 0, 1, 0, matched_pairs);
    if (matched_pairs.size() != 256) {
    }
  }
}

int main(int argc, char *argv[]) {
  Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);
  // mpi_initlog();
  MPI_Comm comm = MPI_COMM_WORLD;
  test<1>(comm);
  test<2>(comm);
  test<3>(comm);

  return 0;
}
