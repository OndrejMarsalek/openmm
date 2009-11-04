
/* Portions copyright (c) 2006-2009 Stanford University and Simbios.
 * Contributors: Pande Group
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __ObcSoftcoreParameters_H__
#define __ObcSoftcoreParameters_H__

#include "../SimTKUtilities/SimTKOpenMMCommon.h"
#include "gbsa/ImplicitSolventParameters.h"

// ---------------------------------------------------------------------------------------

class ObcSoftcoreParameters : public ImplicitSolventParameters {

   public:

       // OBC types

       enum ObcType { ObcTypeI, ObcTypeII };

       static const std::string ParameterFileName;

   private:

      // OBC constants & parameters
   
      RealOpenMM _dielectricOffset;
      RealOpenMM _alphaObc;
      RealOpenMM _betaObc;
      RealOpenMM _gammaObc;

      ObcType _obcType;

      RealOpenMM _nonPolarPreFactor;

     // scaling factors for nonpolar term

      int _ownNonPolarScaleFactors;
      RealOpenMM* _nonPolarScaleFactors;

      // scaled radius factors (S_kk in HCT paper)

      int _ownScaledRadiusFactors;
      RealOpenMM* _scaledRadiusFactors;

      // cutoff and periodic boundary conditions
      
      bool cutoff;
      bool periodic;
      RealOpenMM periodicBoxSize[3];
      RealOpenMM cutoffDistance;

      /**---------------------------------------------------------------------------------------
      
         Set solvent dielectric (Simbios) 
      
         @param dielectricOffset         solvent dielectric
      
         @return SimTKOpenMMCommon::DefaultReturn

         --------------------------------------------------------------------------------------- */
      
      int setDielectricOffset( RealOpenMM dielectricOffset );

   public:

      /**---------------------------------------------------------------------------------------
      
         ObcSoftcoreParameters constructor (Simbios) 
      
         @param numberOfAtoms       number of atoms
      
         --------------------------------------------------------------------------------------- */
      
       ObcSoftcoreParameters( int numberOfAtoms, ObcSoftcoreParameters::ObcType obcType = ObcTypeII );

      /**---------------------------------------------------------------------------------------
      
         ObcSoftcoreParameters destructor (Simbios) 
      
         --------------------------------------------------------------------------------------- */
      
       ~ObcSoftcoreParameters( );

      /**---------------------------------------------------------------------------------------
      
         Get OBC type
      
         @return OBC type
      
         --------------------------------------------------------------------------------------- */
      
      ObcSoftcoreParameters::ObcType getObcType( void ) const;
      
      /**---------------------------------------------------------------------------------------
      
         Set OBC type specific parameters
      
         @param obcType OBC type (ObcTypeI or ObcTypeII -- Eq. 7 or 8)
      
         @return SimTKOpenMMCommon::DefaultReturn
      
         --------------------------------------------------------------------------------------- */
      
      int setObcTypeParameters( ObcSoftcoreParameters::ObcType obcType );
      
      /**---------------------------------------------------------------------------------------
      
         Get alpha OBC (Eqs. 6 & 7) in Proteins paper
      
         @return alphaObc
      
         --------------------------------------------------------------------------------------- */
      
      RealOpenMM getAlphaObc( void ) const;
      
      /**---------------------------------------------------------------------------------------
      
         Get beta OBC (Eqs. 6 & 7) in Proteins paper
      
         @return betaObc
      
         --------------------------------------------------------------------------------------- */
      
      RealOpenMM getBetaObc( void ) const;
      
      /**---------------------------------------------------------------------------------------
      
         Get gamma OBC (Eqs. 6 & 7) in Proteins paper
      
         @return gammaObc
      
         --------------------------------------------------------------------------------------- */
      
      RealOpenMM getGammaObc( void ) const;
      
      /**---------------------------------------------------------------------------------------
      
         Get solvent dielectric (Simbios) 
      
         @return dielectricOffset dielectric offset
      
         --------------------------------------------------------------------------------------- */
      
      RealOpenMM getDielectricOffset( void ) const;

      /**---------------------------------------------------------------------------------------
      
         Return OBC scale factors
      
         @return array
      
         --------------------------------------------------------------------------------------- */
      
      const RealOpenMM* getScaledRadiusFactors( void ) const;
        
      /**---------------------------------------------------------------------------------------
      
         Return OBC scale factors
      
         @return array
      
         --------------------------------------------------------------------------------------- */
      
      int setScaledRadiusFactors( RealOpenMM* scaledRadiusFactors );
#if RealOpenMMType == 2
      int setScaledRadiusFactors( float* scaledRadiusFactors );
#endif
      int setScaledRadiusFactors( const RealOpenMMVector& scaledRadiusFactors );
        
      /**---------------------------------------------------------------------------------------
      
         Set flag indicating whether scale factors arra should be deleted
      
         @param ownScaledRadiusFactors flag indicating whether scale factors 
                                       array should be deleted

         @return SimTKOpenMMCommon::DefaultReturn
      
         --------------------------------------------------------------------------------------- */

      int setOwnScaleFactors( int ownScaledRadiusFactors );
      
      /**--------------------------------------------------------------------------------------- 
      
         Assign standard radii for GB/SA methods other than ACE;
         taken from Macromodel and OPLS-AA, except for hydrogens (Simbios)
      
         Logic based on logic in Tinker's ksolv.f
      
         Currently only works for standard amino acid atoms
         If invalid atom name is encountered, a message is printed to log file and the
         radius for that atom is set to 1.0f
      
         @param numberOfAtoms       number of atoms
         @param atomNames           array of atom names from GMX top data struct
         @param radii               array to store Macromodel radii for each atom
         @param log                 if set, then print error messages to log file
      
         @return SimTKOpenMMCommon::DefaultReturn always
      
         --------------------------------------------------------------------------------------- */
      
      int getMacroModelAtomicRadii( int numberOfAtoms,
                                    char*** atomNames, RealOpenMM* radii, FILE* log );

      /**---------------------------------------------------------------------------------------
      
         Get AtomicRadii array w/ dielectric offset applied
      
         @return array of atom volumes
      
         --------------------------------------------------------------------------------------- */

      RealOpenMM* getAtomicRadii( void ) const;

      /**---------------------------------------------------------------------------------------
      
         Set AtomicRadii array
      
         @param atomicRadii array of atomic radii
      
         @return SimTKOpenMMCommon::DefaultReturn
      
         --------------------------------------------------------------------------------------- */

      int setAtomicRadii( RealOpenMM* atomicRadii );

      /**---------------------------------------------------------------------------------------
      
         Set AtomicRadii array
      
         @param atomicRadii vector of atomic radii
      
         @return SimTKOpenMMCommon::DefaultReturn
      
         --------------------------------------------------------------------------------------- */

      int setAtomicRadii( const RealOpenMMVector& atomicRadii );

      /**---------------------------------------------------------------------------------------
      
         Map Gmx atom name to Tinker atom number (Simbios)
      
         @param atomName            atom name (CA, HA, ...); upper and lower case should both work
         @param log                 if set, then print error messages to log file
      
         return Tinker atom number if atom name is valid; else return -1
      
         --------------------------------------------------------------------------------------- */
            
      int mapGmxAtomNameToTinkerAtomNumber( const char* atomName, FILE* log ) const;

      /**---------------------------------------------------------------------------------------
            
         Get string w/ state
         
         @param title               title (optional)
            
         @return string
            
         --------------------------------------------------------------------------------------- */
      
      std::string getStateString( const char* title ) const;

      /**---------------------------------------------------------------------------------------
                  
         Return zero value if all parameters set; else return nonzero
               
         @return ready status
                  
         --------------------------------------------------------------------------------------- */
       
      int isNotReady( void ) const;

      /**---------------------------------------------------------------------------------------

         Set the force to use a cutoff.

         @param distance            the cutoff distance

         @return SimTKOpenMMCommon::DefaultReturn

         --------------------------------------------------------------------------------------- */

      int setUseCutoff( RealOpenMM distance );

      /**---------------------------------------------------------------------------------------

         Get whether to use a cutoff.

         --------------------------------------------------------------------------------------- */

      bool getUseCutoff();

      /**---------------------------------------------------------------------------------------

         Get the cutoff distance.

         --------------------------------------------------------------------------------------- */

      RealOpenMM getCutoffDistance();

      /**---------------------------------------------------------------------------------------

         Set the force to use periodic boundary conditions.  This requires that a cutoff has
         already been set, and the smallest side of the periodic box is at least twice the cutoff
         distance.

         @param boxSize             the X, Y, and Z widths of the periodic box

         @return SimTKOpenMMCommon::DefaultReturn

         --------------------------------------------------------------------------------------- */

      int setPeriodic( RealOpenMM* boxSize );

      /**---------------------------------------------------------------------------------------

         Get whether to use periodic boundary conditions.

         --------------------------------------------------------------------------------------- */

      bool getPeriodic();

      /**---------------------------------------------------------------------------------------

         Get the periodic box dimension

         --------------------------------------------------------------------------------------- */

      const RealOpenMM* getPeriodicBox();

      /**---------------------------------------------------------------------------------------
      
         Set flag indicating whether scale factors array should be deleted
      
         @param ownNonPolarScaleFactors flag indicating whether scale factors 
                                       array should be deleted
      
         @return SimTKOpenMMCommon::DefaultReturn
      
         --------------------------------------------------------------------------------------- */
      
      int setOwnNonPolarScaleFactors( int ownNonPolarScaleFactors );
      
      /**---------------------------------------------------------------------------------------
      
         Return non-polar scale factors
         If not previously set, allocate space
      
         @return array 
      
         --------------------------------------------------------------------------------------- */
      
      const RealOpenMM* getNonPolarScaleFactors( void ) const;

      /**---------------------------------------------------------------------------------------
      
         Set non-polar scale factors

         @param nonPolarScaleFactors nonpolar scale factors
      
         @return array 
      
         --------------------------------------------------------------------------------------- */
      
      int setNonPolarScaleFactors( const RealOpenMMVector& nonPolarScaleFactors );

      /**---------------------------------------------------------------------------------------
      
         Set nonPolarPrefactor
      
         @param nonPolarPrefactor solute dielectric
      
         @return SimTKOpenMMCommon::DefaultReturn
      
         --------------------------------------------------------------------------------------- */

      int setNonPolarPrefactor( RealOpenMM nonPolarPrefactor );

};
   
/**---------------------------------------------------------------------------------------
      
   Qsort/heapsort integer comparison (Simbios) 
      
   @parma a first value to compare
   @param b second value to compare

   @return -1, 0, 1
      
--------------------------------------------------------------------------------------- */

int integerComparison( const void *a, const void *b);

// ---------------------------------------------------------------------------------------

#endif // __ObcSoftcoreParameters_H__