#ifndef OPENMM_CUSTOMGBFORCE_H_
#define OPENMM_CUSTOMGBFORCE_H_

/* -------------------------------------------------------------------------- *
 *                                   OpenMM                                   *
 * -------------------------------------------------------------------------- *
 * This is part of the OpenMM molecular simulation toolkit originating from   *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org.               *
 *                                                                            *
 * Portions copyright (c) 2008-2009 Stanford University and the Authors.      *
 * Authors: Peter Eastman                                                     *
 * Contributors:                                                              *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included in *
 * all copies or substantial portions of the Software.                        *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR      *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE  *
 * USE OR OTHER DEALINGS IN THE SOFTWARE.                                     *
 * -------------------------------------------------------------------------- */

#include "Force.h"
#include "Vec3.h"
#include <map>
#include <set>
#include <utility>
#include <vector>
#include "internal/windowsExport.h"

namespace OpenMM {

/**
 */

class OPENMM_EXPORT CustomGBForce : public Force {
public:
    /**
     * This is an enumeration of the different methods that may be used for handling long range nonbonded forces.
     */
    enum NonbondedMethod {
        /**
         * No cutoff is applied to nonbonded interactions.  The full set of N^2 interactions is computed exactly.
         * This necessarily means that periodic boundary conditions cannot be used.  This is the default.
         */
        NoCutoff = 0,
        /**
         * Interactions beyond the cutoff distance are ignored.
         */
        CutoffNonPeriodic = 1,
        /**
         * Periodic boundary conditions are used, so that each particle interacts only with the nearest periodic copy of
         * each other particle.  Interactions beyond the cutoff distance are ignored.
         */
        CutoffPeriodic = 2,
    };
    enum ComputationType {
        SingleParticle = 0,
        ParticlePair = 1,
        ParticlePairNoExclusions = 2
    };
    /**
     * Create a CustomGBForce.
     */
    CustomGBForce();
    /**
     * Get the number of particles for which force field parameters have been defined.
     */
    int getNumParticles() const {
        return particles.size();
    }
    /**
     * Get the number of particle pairs whose interactions should be excluded.
     */
    int getNumExclusions() const {
        return exclusions.size();
    }
    /**
     * Get the number of per-particle parameters that the interaction depends on.
     */
    int getNumPerParticleParameters() const {
        return parameters.size();
    }
    /**
     * Get the number of global parameters that the interaction depends on.
     */
    int getNumGlobalParameters() const {
        return globalParameters.size();
    }
    /**
     * Get the number of tabulated functions that have been defined.
     */
    int getNumFunctions() const {
        return functions.size();
    }
    int getNumComputedValues() const {
        return computedValues.size();
    }
    int getNumEnergyTerms() const {
        return energyTerms.size();
    }
    /**
     * Get the method used for handling long range nonbonded interactions.
     */
    NonbondedMethod getNonbondedMethod() const;
    /**
     * Set the method used for handling long range nonbonded interactions.
     */
    void setNonbondedMethod(NonbondedMethod method);
    /**
     * Get the cutoff distance (in nm) being used for nonbonded interactions.  If the NonbondedMethod in use
     * is NoCutoff, this value will have no effect.
     */
    double getCutoffDistance() const;
    /**
     * Set the cutoff distance (in nm) being used for nonbonded interactions.  If the NonbondedMethod in use
     * is NoCutoff, this value will have no effect.
     */
    void setCutoffDistance(double distance);
    /**
     * Add a new per-particle parameter that the interaction may depend on.
     *
     * @param name     the name of the parameter
     * @return the index of the parameter that was added
     */
    int addPerParticleParameter(const std::string& name);
    /**
     * Get the name of a per-particle parameter.
     *
     * @param index     the index of the parameter for which to get the name
     * @return the parameter name
     */
    const std::string& getPerParticleParameterName(int index) const;
    /**
     * Set the name of a per-particle parameter.
     *
     * @param index          the index of the parameter for which to set the name
     * @param name           the name of the parameter
     */
    void setPerParticleParameterName(int index, const std::string& name);
    /**
     * Add a new global parameter that the interaction may depend on.
     *
     * @param name             the name of the parameter
     * @param defaultValue     the default value of the parameter
     * @return the index of the parameter that was added
     */
    int addGlobalParameter(const std::string& name, double defaultValue);
    /**
     * Get the name of a global parameter.
     *
     * @param index     the index of the parameter for which to get the name
     * @return the parameter name
     */
    const std::string& getGlobalParameterName(int index) const;
    /**
     * Set the name of a global parameter.
     *
     * @param index          the index of the parameter for which to set the name
     * @param name           the name of the parameter
     */
    void setGlobalParameterName(int index, const std::string& name);
    /**
     * Get the default value of a global parameter.
     *
     * @param index     the index of the parameter for which to get the default value
     * @return the parameter default value
     */
    double getGlobalParameterDefaultValue(int index) const;
    /**
     * Set the default value of a global parameter.
     *
     * @param index          the index of the parameter for which to set the default value
     * @param name           the default value of the parameter
     */
    void setGlobalParameterDefaultValue(int index, double defaultValue);
    /**
     * Add the nonbonded force parameters for a particle.  This should be called once for each particle
     * in the System.  When it is called for the i'th time, it specifies the parameters for the i'th particle.
     *
     * @param parameters    the list of parameters for the new particle
     * @return the index of the particle that was added
     */
    int addParticle(const std::vector<double>& parameters);
    /**
     * Get the nonbonded force parameters for a particle.
     *
     * @param index       the index of the particle for which to get parameters
     * @param parameters  the list of parameters for the specified particle
     */
    void getParticleParameters(int index, std::vector<double>& parameters) const;
    /**
     * Set the nonbonded force parameters for a particle.
     *
     * @param index       the index of the particle for which to set parameters
     * @param parameters  the list of parameters for the specified particle
     */
    void setParticleParameters(int index, const std::vector<double>& parameters);
    int addComputedValue(const std::string& name, const std::string& expression, ComputationType type);
    void getComputedValueParameters(int index, std::string& name, std::string& expression, ComputationType& type) const;
    void setComputedValueParameters(int index, const std::string& name, const std::string& expression, ComputationType type);
    int addEnergyTerm(const std::string& expression, ComputationType type);
    void getEnergyTermParameters(int index, std::string& expression, ComputationType& type) const;
    void setEnergyTermParameters(int index, const std::string& expression, ComputationType type);
    /**
     * Add a particle pair to the list of interactions that should be excluded.
     *
     * @param particle1  the index of the first particle in the pair
     * @param particle2  the index of the second particle in the pair
     * @return the index of the exclusion that was added
     */
    int addExclusion(int particle1, int particle2);
    /**
     * Get the particles in a pair whose interaction should be excluded.
     *
     * @param index      the index of the exclusion for which to get particle indices
     * @param particle1  the index of the first particle in the pair
     * @param particle2  the index of the second particle in the pair
     */
    void getExclusionParticles(int index, int& particle1, int& particle2) const;
    /**
     * Set the particles in a pair whose interaction should be excluded.
     *
     * @param index      the index of the exclusion for which to set particle indices
     * @param particle1  the index of the first particle in the pair
     * @param particle2  the index of the second particle in the pair
     */
    void setExclusionParticles(int index, int particle1, int particle2);
    /**
     * Add a tabulated function that may appear in the energy expression.
     *
     * @param name           the name of the function as it appears in expressions
     * @param values         the tabulated values of the function f(x) at uniformly spaced values of x between min and max.
     *                       The function is assumed to be zero for x &lt; min or x &gt; max.
     * @param min            the value of the independent variable corresponding to the first element of values
     * @param max            the value of the independent variable corresponding to the last element of values
     * @param interpolating  if true, an interpolating (Catmull-Rom) spline will be used to represent the function.
     *                       If false, an approximating spline (B-spline) will be used.
     * @return the index of the function that was added
     */
    int addFunction(const std::string& name, const std::vector<double>& values, double min, double max, bool interpolating);
    /**
     * Get the parameters for a tabulated function that may appear in the energy expression.
     *
     * @param index          the index of the function for which to get parameters
     * @param name           the name of the function as it appears in expressions
     * @param values         the tabulated values of the function f(x) at uniformly spaced values of x between min and max.
     *                       The function is assumed to be zero for x &lt; min or x &gt; max.
     * @param min            the value of the independent variable corresponding to the first element of values
     * @param max            the value of the independent variable corresponding to the last element of values
     * @param interpolating  if true, an interpolating (Catmull-Rom) spline will be used to represent the function.
     *                       If false, an approximating spline (B-spline) will be used.
     */
    void getFunctionParameters(int index, std::string& name, std::vector<double>& values, double& min, double& max, bool& interpolating) const;
    /**
     * Set the parameters for a tabulated function that may appear in algebraic expressions.
     *
     * @param index          the index of the function for which to set parameters
     * @param name           the name of the function as it appears in expressions
     * @param values         the tabulated values of the function f(x) at uniformly spaced values of x between min and max.
     *                       The function is assumed to be zero for x &lt; min or x &gt; max.
     * @param min            the value of the independent variable corresponding to the first element of values
     * @param max            the value of the independent variable corresponding to the last element of values
     * @param interpolating  if true, an interpolating (Catmull-Rom) spline will be used to represent the function.
     *                       If false, an approximating spline (B-spline) will be used.
     */
    void setFunctionParameters(int index, const std::string& name, const std::vector<double>& values, double min, double max, bool interpolating);
protected:
    ForceImpl* createImpl();
private:
    class ParticleInfo;
    class PerParticleParameterInfo;
    class GlobalParameterInfo;
    class ExclusionInfo;
    class FunctionInfo;
    class ComputationInfo;
    NonbondedMethod nonbondedMethod;
    double cutoffDistance;
    std::string energyExpression;
    std::vector<PerParticleParameterInfo> parameters;
    std::vector<GlobalParameterInfo> globalParameters;
    std::vector<ParticleInfo> particles;
    std::vector<ExclusionInfo> exclusions;
    std::vector<FunctionInfo> functions;
    std::vector<ComputationInfo> computedValues;
    std::vector<ComputationInfo> energyTerms;
};

class CustomGBForce::ParticleInfo {
public:
    std::vector<double> parameters;
    ParticleInfo() {
    }
    ParticleInfo(const std::vector<double>& parameters) : parameters(parameters) {
    }
};

class CustomGBForce::PerParticleParameterInfo {
public:
    std::string name;
    PerParticleParameterInfo() {
    }
    PerParticleParameterInfo(const std::string& name) : name(name) {
    }
};

class CustomGBForce::GlobalParameterInfo {
public:
    std::string name;
    double defaultValue;
    GlobalParameterInfo() {
    }
    GlobalParameterInfo(const std::string& name, double defaultValue) : name(name), defaultValue(defaultValue) {
    }
};

class CustomGBForce::ExclusionInfo {
public:
    int particle1, particle2;
    ExclusionInfo() {
        particle1 = particle2 = -1;
    }
    ExclusionInfo(int particle1, int particle2) :
        particle1(particle1), particle2(particle2) {
    }
};

class CustomGBForce::FunctionInfo {
public:
    std::string name;
    std::vector<double> values;
    double min, max;
    bool interpolating;
    FunctionInfo() {
    }
    FunctionInfo(const std::string& name, const std::vector<double>& values, double min, double max, bool interpolating) :
        name(name), values(values), min(min), max(max), interpolating(interpolating) {
    }
};

class CustomGBForce::ComputationInfo {
public:
    std::string name;
    std::string expression;
    CustomGBForce::ComputationType type;
    ComputationInfo() {
    }
    ComputationInfo(const std::string& name, const std::string& expression, CustomGBForce::ComputationType type) :
        name(name), expression(expression), type(type) {
    }
};

} // namespace OpenMM

#endif /*OPENMM_CUSTOMGBFORCE_H_*/