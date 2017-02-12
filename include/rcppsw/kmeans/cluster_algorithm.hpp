/*******************************************************************************
 * Name            : cluster_algorithm.hpp
 * Project         : rcppsw
 * Module          : kmeans
 * Description     : Base class implementation of K-means clustering algorithm
 * Creation Date   : 02/02/17
 * Copyright       : Copyright 2017 John Harwell, All rights reserved
 *
 ******************************************************************************/

#ifndef INCLUDE_RCPPSW_KMEANS_CLUSTER_ALGORITHM_HPP_
#define INCLUDE_RCPPSW_KMEANS_CLUSTER_ALGORITHM_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <time.h>
#include <algorithm>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "rcppsw/kmeans/cluster.hpp"
#include "rcppsw/erf_client.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace rcppsw {
namespace kmeans {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
template <typename T> class cluster_algorithm: public erf_client {
 public:
  /* constructors */
  cluster_algorithm(std::size_t n_iterations,
                    std::size_t n_clusters,
                    std::size_t n_threads,
                    std::size_t dimension,
                    std::size_t n_points,
                    const std::string& clusters_fname,
                    const std::string& centroids_fname,
                    er_frmwk *const erf) :
      erf_client(erf),
      n_iterations_(n_iterations), n_clusters_(n_clusters),
      n_threads_(n_threads),
      dimension_(dimension), n_points_(n_points),
      data_(NULL),
      membership_(NULL),
      clusters_fname_(clusters_fname),
      centroids_fname_(centroids_fname),
      clusters_(new std::vector<kmeans_cluster<T>*>()) {
    erf_handle()->insmod(erf_id(), "KMEANS");
    ER_REPORT(erf_lvl::NOM, "n_points=%lu, n_clusters=%lu, n_iterations=%lu\n",
              n_points_, n_clusters_, n_iterations_);
  }

  virtual ~cluster_algorithm(void) {
    for (std::size_t i = 0; i < n_clusters_; ++i) {
      delete(clusters_->at(i));
    } /* for(i..) */
    free(membership_);
  }

  /* member functions */
  boost::shared_ptr<std::vector<kmeans_cluster<T>*>>& clusters(void) {
    return clusters_;
  }
  std::size_t n_clusters(void) { return n_clusters_; }
  std::size_t n_threads(void) { return n_threads_; }
  T* data(void) { return data_; }
  void data(T* in) { data_ = in; }
  std::size_t* membership(void) { return membership_; }
  void membership(std::size_t* in) { membership_ = in; }
  std::size_t n_points(void) { return n_points_; }
  std::size_t dimension(void) { return dimension_; }

  virtual void report_clusters(void) {
    std::ofstream ofile(clusters_fname_);
    for (std::size_t j = 0; j < n_points_; ++j) {
      ofile << membership_[j] << std::endl;
    } /* for(j..) */
  }
  virtual void report_centroids(void) {
    std::ofstream ofile(centroids_fname_);
    ofile << clusters_->size() << " " << dimension_ << std::endl;
    std::for_each(clusters_->begin(), clusters_->end(),
                  [&](const kmeans_cluster<T>* c) {
                    c->report_center(ofile);
                  });
  }
  void cluster(void) {
    ER_REPORT(erf_lvl::NOM, "Begin clustering\n");
    double end = 0.0;
    double start = monotonic_seconds();
    for (std::size_t i = 0; i < n_iterations_; ++i) {
      double iter_start = monotonic_seconds();
      if (cluster_iterate()) {
        ER_REPORT(erf_lvl::NOM, "Clusters report convergence: terminating\n");
        end = monotonic_seconds();
        break;
      } else {
        end = monotonic_seconds();
      }
      ER_REPORT(erf_lvl::DIAG, "Iteration %lu time: %.8fms\n", i, (end - iter_start)*1000);
    } /* for(i..) */

    ER_REPORT(erf_lvl::NOM,"k-means clustering time: %0.04fs\n", end-start);
  } /* cluster_algorithm::cluster() */

  virtual void initialize(std::vector<multidim_point<T>>* data_in)  {
    /* allocate contiguous memory */
    std::size_t* data_block = (std::size_t*)malloc(sizeof(T) * n_points_ * dimension_
                                                   + sizeof(std::size_t) * n_points_);
    assert(NULL != data_block);
    data_ = reinterpret_cast<T*>(data_block + n_points_);
    membership_ = data_block;

    first_touch_allocation();

    /* Copy data into contiguous chunk */
    for (std::size_t i = 0; i < n_points_; ++i) {
      std::copy(data_in->at(i).begin(), data_in->at(i).end(),
                data_ + (i * dimension_));
    } /* for(i..) */

    /*
     * Initialize all clusters, including centers
     */
    for (std::size_t i = 0; i < n_clusters_; ++i) {
      clusters_->emplace_back(new kmeans_cluster<T>(i, dimension_, n_points_,
                                                    data_, membership_));
    } /* for(i..) */
  }
  virtual void first_touch_allocation(void) {}

 protected:
  virtual bool cluster_iterate(void) = 0;

 private:
  /* member functions */
  cluster_algorithm(const cluster_algorithm& other) = delete;
  cluster_algorithm& operator=(const cluster_algorithm& other) = delete;
  double monotonic_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
  }

  /* data members */
  std::size_t n_iterations_;
  std::size_t n_clusters_;
  std::size_t n_threads_;

  /*
   * Dimension is 1 more than the actual data dimension, to efficiently store
   * the cluster associated with each data point.
   */
  std::size_t dimension_;
  std::size_t n_points_;
  T* data_;
  std::size_t* membership_;
  const std::string& clusters_fname_;
  const std::string& centroids_fname_;
  boost::shared_ptr<std::vector<kmeans_cluster<T>*>> clusters_;
};

} /* namespace kmeans */
} /* namespace rcppsw */

#endif /* INCLUDE_RCPPSW_KMEANS_CLUSTER_ALGORITHM_HPP_ */