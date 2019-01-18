!
! CDDL HEADER START
!
! The contents of this file are subject to the terms of the Common Development
! and Distribution License Version 1.0 (the "License").
!
! You can obtain a copy of the license at
! http://www.opensource.org/licenses/CDDL-1.0.  See the License for the
! specific language governing permissions and limitations under the License.
!
! When distributing Covered Code, include this CDDL HEADER in each file and
! include the License file in a prominent location with the name LICENSE.CDDL.
! If applicable, add the following below this CDDL HEADER, with the fields
! enclosed by brackets "[]" replaced with your own identifying information:
!
! Portions Copyright (c) [yyyy] [name of copyright owner]. All rights reserved.
!
! CDDL HEADER END
!

!
! Copyright (c) 2016--2019, Regents of the University of Minnesota.
! All rights reserved.
!
! Contributors:
!    Ryan S. Elliott
!

!
! Release: This file is part of the kim-api.git repository.
!


module kim_compute_callback_name_module
  use, intrinsic :: iso_c_binding
  implicit none
  private

  public &
    ! Derived types
    kim_compute_callback_name_type, &

    ! Constants
    KIM_COMPUTE_CALLBACK_NAME_GET_NEIGHBOR_LIST, &
    KIM_COMPUTE_CALLBACK_NAME_PROCESS_DEDR_TERM, &
    KIM_COMPUTE_CALLBACK_NAME_PROCESS_D2EDR2_TERM, &

    ! Routines
    kim_known, &
    operator (.eq.), &
    operator (.ne.), &
    kim_from_string, &
    kim_to_string, &
    kim_get_number_of_compute_callback_names, &
    kim_get_compute_callback_name


  type, bind(c) :: kim_compute_callback_name_type
    integer(c_int) compute_callback_name_id
  end type kim_compute_callback_name_type

  type(kim_compute_callback_name_type), protected, save, &
    bind(c, name="KIM_COMPUTE_CALLBACK_NAME_GetNeighborList") &
    :: KIM_COMPUTE_CALLBACK_NAME_GET_NEIGHBOR_LIST
  type(kim_compute_callback_name_type), protected, save, &
    bind(c, name="KIM_COMPUTE_CALLBACK_NAME_ProcessDEDrTerm") &
    :: KIM_COMPUTE_CALLBACK_NAME_PROCESS_DEDR_TERM
  type(kim_compute_callback_name_type), protected, save, &
    bind(c, name="KIM_COMPUTE_CALLBACK_NAME_ProcessD2EDr2Term") &
    :: KIM_COMPUTE_CALLBACK_NAME_PROCESS_D2EDR2_TERM

  interface kim_known
    module procedure kim_compute_callback_name_known
  end interface kim_known

  interface operator (.eq.)
    module procedure kim_compute_callback_name_equal
  end interface operator (.eq.)

  interface operator (.ne.)
    module procedure kim_compute_callback_name_not_equal
  end interface operator (.ne.)

  interface kim_from_string
    module procedure kim_compute_callback_name_from_string
  end interface kim_from_string

  interface kim_to_string
    module procedure kim_compute_callback_name_to_string
  end interface kim_to_string

contains
  logical recursive function kim_compute_callback_name_known( &
    compute_callback_name)
    implicit none
    interface
      integer(c_int) recursive function known(compute_callback_name) &
        bind(c, name="KIM_ComputeCallbackName_Known")
        use, intrinsic :: iso_c_binding
        import kim_compute_callback_name_type
        implicit none
        type(kim_compute_callback_name_type), intent(in), value :: &
          compute_callback_name
      end function known
    end interface
    type(kim_compute_callback_name_type), intent(in) :: compute_callback_name

    kim_compute_callback_name_known = (known(compute_callback_name) /= 0)
  end function kim_compute_callback_name_known

  logical recursive function kim_compute_callback_name_equal(lhs, rhs)
    implicit none
    type(kim_compute_callback_name_type), intent(in) :: lhs
    type(kim_compute_callback_name_type), intent(in) :: rhs

    kim_compute_callback_name_equal &
      = (lhs%compute_callback_name_id .eq. rhs%compute_callback_name_id)
  end function kim_compute_callback_name_equal

  logical recursive function kim_compute_callback_name_not_equal(lhs, rhs)
    implicit none
    type(kim_compute_callback_name_type), intent(in) :: lhs
    type(kim_compute_callback_name_type), intent(in) :: rhs

    kim_compute_callback_name_not_equal = .not. (lhs .eq. rhs)
  end function kim_compute_callback_name_not_equal

  recursive subroutine kim_compute_callback_name_from_string(string, &
    compute_callback_name)
    implicit none
    interface
      type(kim_compute_callback_name_type) recursive function from_string( &
        string) bind(c, name="KIM_ComputeCallbackName_FromString")
        use, intrinsic :: iso_c_binding
        import kim_compute_callback_name_type
        implicit none
        character(c_char), intent(in) :: string(*)
      end function from_string
    end interface
    character(len=*, kind=c_char), intent(in) :: string
    type(kim_compute_callback_name_type), intent(out) :: compute_callback_name

    compute_callback_name = from_string(trim(string)//c_null_char)
  end subroutine kim_compute_callback_name_from_string

  recursive subroutine kim_compute_callback_name_to_string( &
    compute_callback_name, string)
    use kim_convert_string_module, only : kim_convert_c_char_ptr_to_string
    implicit none
    interface
      type(c_ptr) recursive function get_string(compute_callback_name) &
        bind(c, name="KIM_ComputeCallbackName_ToString")
        use, intrinsic :: iso_c_binding
        import kim_compute_callback_name_type
        implicit none
        type(kim_compute_callback_name_type), intent(in), value :: &
          compute_callback_name
      end function get_string
    end interface
    type(kim_compute_callback_name_type), intent(in) :: &
      compute_callback_name
    character(len=*, kind=c_char), intent(out) :: string

    type(c_ptr) :: p

    p = get_string(compute_callback_name)
    call kim_convert_c_char_ptr_to_string(p, string)
  end subroutine kim_compute_callback_name_to_string

  recursive subroutine kim_get_number_of_compute_callback_names( &
    number_of_compute_callback_names)
    implicit none
    interface
      recursive subroutine get_number_of_compute_callback_names( &
        number_of_compute_callback_names) &
        bind(c, &
        name="KIM_COMPUTE_CALLBACK_NAME_GetNumberOfComputeCallbackNames")
        use, intrinsic :: iso_c_binding
        integer(c_int), intent(out) :: number_of_compute_callback_names
      end subroutine get_number_of_compute_callback_names
    end interface
    integer(c_int), intent(out) :: number_of_compute_callback_names

    call get_number_of_compute_callback_names(number_of_compute_callback_names)
  end subroutine kim_get_number_of_compute_callback_names

  recursive subroutine kim_get_compute_callback_name(index, &
    compute_callback_name, ierr)
    implicit none
    interface
      integer(c_int) recursive function get_compute_callback_name(index, &
        compute_callback_name) &
        bind(c, name="KIM_COMPUTE_CALLBACK_NAME_GetComputeCallbackName")
        use, intrinsic :: iso_c_binding
        import kim_compute_callback_name_type
        implicit none
        integer(c_int), intent(in), value :: index
        type(kim_compute_callback_name_type), intent(out) :: &
          compute_callback_name
      end function get_compute_callback_name
    end interface
    integer(c_int), intent(in) :: index
    type(kim_compute_callback_name_type), intent(out) :: compute_callback_name
    integer(c_int), intent(out) :: ierr

    ierr = get_compute_callback_name(index-1, compute_callback_name)
  end subroutine kim_get_compute_callback_name
end module kim_compute_callback_name_module
