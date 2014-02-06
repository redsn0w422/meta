/**
 * @file absolute_discount.h
 *
 * All files in META are released under the MIT license. For more details,
 * consult the file LICENSE in the root of the project.
 *
 * @author Sean Massung
 */

#ifndef _ABSOLUTE_DISCOUNT_H_
#define _ABSOLUTE_DISCOUNT_H_

#include "index/ranker/lm_ranker.h"

namespace meta
{
namespace index
{

/**
 * Implements the absolute discounting smoothing method.
 */
class absolute_discount : public language_model_ranker
{
  public:
    /**
     * @param delta
     */
    absolute_discount(double delta = 0.7);

    /**
     * Calculates the smoothed probability of a term.
     * @param sd
     */
    double smoothed_prob(const score_data& sd) const override;

    /**
     * A document-dependent constant.
     * @param sd
     */
    double doc_constant(const score_data& sd) const override;

  private:
    /** the absolute discounting parameter */
    const double _delta;
};
}
}

#endif