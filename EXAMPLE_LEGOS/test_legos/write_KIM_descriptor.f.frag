!-------------------------------------------------------------------------------
!
!  Write KIM descriptor file for MiniMol for given NBC and set of
!  support species types
!
!-------------------------------------------------------------------------------
subroutine Write_KIM_descriptor(NBC_method, max_types, model_types, num_types, &
                                kim_descriptor, ier)
use KIM_API
implicit none

!-- Transferred variables
character(len=KIM_KEY_STRING_LENGTH), intent(in)   :: NBC_method
integer,                              intent(in)   :: max_types
character(len=KIM_KEY_STRING_LENGTH), intent(in)   :: model_types(max_types)
integer,                              intent(in)   :: num_types
character(len=10000),                 intent(out)  :: kim_descriptor
integer,                              intent(out)  :: ier

!-- Local variables
integer :: i
character(len=103) :: divider
character(len=1)   :: cr
character(len=52)  :: type_line
character(len=64)  :: type64
character(len=32)  :: nbcline

! Initialize error flag
ier = KIM_STATUS_OK

! Define frequently used variables
!
cr = char(10)
divider = '#######################################################################################################'

! Write Minimol descriptor file into string kim_descriptor
!
kim_descriptor = &
   divider                                                                      // cr // &
   '#'                                                                          // cr // &
   '# CDDL HEADER START'                                                        // cr // &
   '#'                                                                          // cr // &
   '# The contents of this file are subject to the terms of the Common Development' // cr // &
   '# and Distribution License Version 1.0 (the "License").'                    // cr // &
   '#'                                                                          // cr // &
   '# You can obtain a copy of the license at'                                  // cr // &
   '# http://www.opensource.org/licenses/CDDL-1.0.  See the License for the'    // cr // &
   '# specific language governing permissions and limitations under the License.' // cr // &
   '#'                                                                          // cr // &
   '# When distributing Covered Code, include this CDDL HEADER in each file and'// cr // &
   '# include the License file in a prominent location with the name LICENSE.CDDL.' // cr // &
   '# If applicable, add the following below this CDDL HEADER, with the fields' // cr // &
   '# enclosed by brackets "[]" replaced with your own identifying information:'// cr // &
   '#'                                                                          // cr // &
   '# Portions Copyright (c) [yyyy] [name of copyright owner]. All rights reserved.' // cr // &
   '#'                                                                          // cr // &
   '# CDDL HEADER END'                                                          // cr // &
   '#'                                                                          // cr // &
                                                                                   cr // &
   '#'                                                                          // cr // &
   '# Copyright (c) 2012, Regents of the University of Minnesota.'              // cr // &
   '# All rights reserved.'                                                     // cr // &
   '#'                                                                          // cr // &
   '# Contributors:'                                                            // cr // &
   '#    Automatically generated by calling Test'                               // cr // &
   '#'                                                                          // cr // &
                                                                                   cr // &
   '#'                                                                          // cr // &
   '# See KIM_API/standard.kim for documentation about this file'               // cr // &
   '#'                                                                          // cr // &
   divider                                                                      // cr // &
                                                                                   cr // &
                                                                                   cr // &
   'Unit_length      := A'                                                      // cr // &
   'Unit_energy      := eV'                                                     // cr // &
   'Unit_charge      := e'                                                      // cr // &
   'Unit_temperature := K'                                                      // cr // &
   'Unit_time        := ps'                                                     // cr // &
                                                                                   cr // &
                                                                                   cr // &
   divider                                                                      // cr // &
   'SUPPORTED_ATOM/PARTICLES_TYPES:'                                            // cr // &
   '# Symbol/name               Type                    code'                   // cr

   do i = 1,num_types
      type64 = model_types(i)
      write(type_line,'(a65,''spec'',20x,i4)') type64,0
      kim_descriptor = trim(kim_descriptor) // type_line // cr
   enddo

   nbcline = NBC_method(1:28) // 'flag'

   kim_descriptor = trim(kim_descriptor) // &
                                                                                   cr // &
                                                                                   cr // &
   divider                                                                      // cr // &
   'CONVENTIONS:'                                                               // cr // &
   '# Name                      Type'                                           // cr // &
                                                                                   cr // &
   'OneBasedLists               flag'                                           // cr // &
                                                                                   cr // &
   'Neigh_BothAccess            flag'                                           // cr // &
                                                                                   cr // &
   nbcline                                                                      // cr // &
                                                                                   cr // &
   divider                                                                      // cr // &
   'MODEL_INPUT:'                                                               // cr // &
   '# Name                      Type         Unit       Shape              requirements' // cr // &
   'numberOfParticles           integer      none       []'                     // cr // &
                                                                                   cr // &
   'numberParticleTypes         integer      none       []'                     // cr // &
                                                                                   cr // &
   'particleTypes               integer      none       [numberOfParticles]'    // cr // &
                                                                                   cr // &
   'coordinates                 real*8       length     [numberOfParticles,3]'  // cr // &
                                                                                   cr // &
   'get_neigh                   method       none       []'                     // cr // &
                                                                                   cr // &
   'neighObject                 pointer      none       []'                     // cr // &
                                                                                   cr // &
   'numberContributingParticles integer      none       []'                     // cr // &
                                                                                   cr // &
   'boxSideLengths              real*8       length     [3]'                    // cr // &
                                                                                   cr // &
                                                                                   cr // &
   divider                                                                      // cr // &
   'MODEL_OUTPUT:'                                                              // cr // &
   '# Name                      Type         Unit       Shape              requirements' // cr // &
                                                                                   cr // &
   'destroy                     method       none       []'                     // cr // &
                                                                                   cr // &
   'compute                     method       none       []'                     // cr // &
                                                                                   cr // &
   'cutoff                      real*8       length     []'                     // cr // &
                                                                                   cr // &
   'energy                      real*8       energy     []'                     // cr // &
                                                                                   cr // &
   'forces                      real*8       force      [numberOfParticles,3]'  // cr // &
                                                                                   cr // &
   divider                                                                            // cr

return

end subroutine Write_KIM_descriptor
