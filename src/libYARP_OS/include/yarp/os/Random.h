/*
 * Copyright (C) 2006 RobotCub Consortium
 * Authors: Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#ifndef YARP_OS_RANDOM_H
#define YARP_OS_RANDOM_H

#ifndef YARP_NO_DEPRECATED // Since YARP 2.3.72

#include <yarp/os/api.h>

/**
 * \file Random.h Contains methods and classes for generating random
 * numbers with various (simple) distributions.
 */


namespace yarp {
    namespace os {
        class Random;
    }
}

/**
 * \ingroup key_class
 *
 * A collection of basic random number generation algorithms.
 */
class YARP_OS_API YARP_OS_DEPRECATED_API_MSG("yarp::os::Random API is deprecated, use instead utilities from C++11's <random> lib") yarp::os::Random {


public:
    /**
     * Generates a random number in the range 0 to 1.
     * @return a random floating point number in the range 0 to 1.
     */
    static double uniform();

    /**
     * A normal random number generator.
     * @param m is the mean of the gaussian distribution.
     * @param s is the standard deviation of the distribution.
     * @return a random number with normal distribution.
     */
    static double normal(double m, double s);

    /**
     * Sets the seed of the random number generator.
     * @param seed is the new seed of the random number generator.
     */
    static void seed(int seed);

    /**
     * @return a random number normally distributed with 0 mean and
     * unitary standard deviation.
     */
    static double normal();

    /**
     * A random number in a specified range.
     * @param min is the minimum of the range.
     * @param max is the maximum of the range.
     * @return a random integer in the specified range.
     */
    static int uniform(int min, int max);
};

#endif // YARP_NO_DEPRECATED

#endif // YARP_OS_RANDOM_H
