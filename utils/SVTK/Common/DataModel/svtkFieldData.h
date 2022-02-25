/*=========================================================================

  Program:   Visualization Toolkit
  Module:    svtkFieldData.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   svtkFieldData
 * @brief   represent and manipulate fields of data
 *
 * svtkFieldData represents and manipulates fields of data. The model of a field
 * is a m x n matrix of data values, where m is the number of tuples, and n
 * is the number of components. (A tuple is a row of n components in the
 * matrix.) The field is assumed to be composed of a set of one or more data
 * arrays, where the data in the arrays are of different types (e.g., int,
 * double, char, etc.), and there may be variable numbers of components in
 * each array. Note that each data array is assumed to be "m" in length
 * (i.e., number of tuples), which typically corresponds to the number of
 * points or cells in a dataset. Also, each data array must have a
 * character-string name. (This is used to manipulate data.)
 *
 * There are two ways of manipulating and interfacing to fields. You can do
 * it generically by manipulating components/tuples via a double-type data
 * exchange, or you can do it by grabbing the arrays and manipulating them
 * directly. The former is simpler but performs type conversion, which is bad
 * if your data has non-castable types like (void) pointers, or you lose
 * information as a result of the cast. The, more efficient method means
 * managing each array in the field.  Using this method you can create
 * faster, more efficient algorithms that do not lose information.
 *
 * @sa
 * svtkAbstractArray svtkDataSetAttributes svtkPointData svtkCellData
 */

#ifndef svtkFieldData_h
#define svtkFieldData_h

#include "svtkCommonDataModelModule.h" // For export macro
#include "svtkObject.h"

#include "svtkAbstractArray.h" // Needed for inline methods.

class svtkIdList;

class SVTKCOMMONDATAMODEL_EXPORT svtkFieldData : public svtkObject
{
public:
  static svtkFieldData* New();

  svtkTypeMacro(svtkFieldData, svtkObject);
  void PrintSelf(ostream& os, svtkIndent indent) override;

  /**
   * Release all data but do not delete object.
   * Also, clear the copy flags.
   */
  virtual void Initialize();

  /**
   * Allocate data for each array.
   * Note that ext is no longer used.
   */
  svtkTypeBool Allocate(svtkIdType sz, svtkIdType ext = 1000);

  /**
   * Copy data array structure from a given field.  The same arrays
   * will exist with the same types, but will contain nothing in the
   * copy.
   */
  void CopyStructure(svtkFieldData*);

  /**
   * AllocateOfArrays actually sets the number of
   * svtkAbstractArray pointers in the svtkFieldData object, not the
   * number of used pointers (arrays). Adding more arrays will
   * cause the object to dynamically adjust the number of pointers
   * if it needs to extend. Although AllocateArrays can
   * be used if the number of arrays which will be added is
   * known, it can be omitted with a small computation cost.
   */
  void AllocateArrays(int num);

  /**
   * Get the number of arrays of data available.
   * This does not include nullptr array pointers therefore after
   * fd->AllocateArray(n); nArrays = GetNumberOfArrays()
   * nArrays is not necessarily equal to n.
   */
  int GetNumberOfArrays() { return this->NumberOfActiveArrays; }

  /**
   * Add an array to the array list. If an array with the same name
   * already exists - then the added array will replace it.
   * Return the index of the added array.
   */
  int AddArray(svtkAbstractArray* array);

  //@{
  /**
   * Remove an array (with the given name or index) from the list of arrays.
   */
  virtual void RemoveArray(const char* name);
  virtual void RemoveArray(int index);
  //@}

  /**
   * Not recommended for use. Use GetAbstractArray(int i) instead.
   *
   * Return the ith array in the field. A nullptr is returned if the
   * index i is out of range, or if the array at the given
   * index is not a svtkDataArray. To access svtkStringArray,
   * svtkUnicodeStringArray, or svtkVariantArray, use GetAbstractArray(int i).
   */
  svtkDataArray* GetArray(int i);

  /**
   * Not recommended for use. Use
   * GetAbstractArray(const char *arrayName, int &index) instead.
   *
   * Return the array with the name given. Returns nullptr if array not found.
   * A nullptr is also returned if the array with the given name is not a
   * svtkDataArray. To access svtkStringArray, svtkUnicodeStringArray, or
   * svtkVariantArray, use GetAbstractArray(const char* arrayName, int &index).
   * Also returns the index of the array if found, -1 otherwise.
   */
  svtkDataArray* GetArray(const char* arrayName, int& index);

  //@{
  /**
   * Not recommended for use. Use GetAbstractArray(const char *arrayName)
   * instead.
   *
   * Return the array with the name given. Returns nullptr if array not found.
   * A nullptr is also returned if the array with the given name is not a
   * svtkDataArray. To access svtkStringArray, svtkUnicodeStringArray, or
   * svtkVariantArray, use GetAbstractArray(const char *arrayName).
   */
  svtkDataArray* GetArray(const char* arrayName)
  {
    int i;
    return this->GetArray(arrayName, i);
  }
  //@}

  /**
   * Returns the ith array in the field. Unlike GetArray(), this method returns
   * a svtkAbstractArray and can be used to access any array type. A nullptr is
   * returned only if the index i is out of range.
   */
  svtkAbstractArray* GetAbstractArray(int i);

  /**
   * Return the array with the name given. Returns nullptr if array not found.
   * Unlike GetArray(), this method returns a svtkAbstractArray and can be used
   * to access any array type. Also returns index of array if found, -1
   * otherwise.
   */
  svtkAbstractArray* GetAbstractArray(const char* arrayName, int& index);

  //@{
  /**
   * Return the array with the name given. Returns nullptr if array not found.
   * Unlike GetArray(), this method returns a svtkAbstractArray and can be used
   * to access any array type.
   */
  svtkAbstractArray* GetAbstractArray(const char* arrayName)
  {
    int i;
    return this->GetAbstractArray(arrayName, i);
  }
  //@}

  //@{
  /**
   * Return 1 if an array with the given name could be found. 0 otherwise.
   */
  int HasArray(const char* name)
  {
    int i;
    svtkAbstractArray* array = this->GetAbstractArray(name, i);
    // assert( i == -1);
    return array ? 1 : 0;
  }
  //@}

  //@{
  /**
   * Get the name of ith array.
   * Note that this is equivalent to:
   * GetAbstractArray(i)->GetName() if ith array pointer is not nullptr
   */
  const char* GetArrayName(int i)
  {
    svtkAbstractArray* da = this->GetAbstractArray(i);
    return da ? da->GetName() : nullptr;
  }
  //@}

  /**
   * Pass entire arrays of input data through to output. Obey the "copy"
   * flags.
   */
  virtual void PassData(svtkFieldData* fd);

  /**
   * Turn on/off the copying of the field specified by name.
   * During the copying/passing, the following rules are followed for each
   * array:
   * 1. If the copy flag for an array is set (on or off), it is applied
   * This overrides rule 2.
   * 2. If CopyAllOn is set, copy the array.
   * If CopyAllOff is set, do not copy the array
   */
  void CopyFieldOn(const char* name) { this->CopyFieldOnOff(name, 1); }
  void CopyFieldOff(const char* name) { this->CopyFieldOnOff(name, 0); }

  /**
   * Turn on copying of all data.
   * During the copying/passing, the following rules are followed for each
   * array:
   * 1. If the copy flag for an array is set (on or off), it is applied
   * This overrides rule 2.
   * 2. If CopyAllOn is set, copy the array.
   * If CopyAllOff is set, do not copy the array
   */
  virtual void CopyAllOn(int unused = 0);

  /**
   * Turn off copying of all data.
   * During the copying/passing, the following rules are followed for each
   * array:
   * 1. If the copy flag for an array is set (on or off), it is applied
   * This overrides rule 2.
   * 2. If CopyAllOn is set, copy the array.
   * If CopyAllOff is set, do not copy the array
   */
  virtual void CopyAllOff(int unused = 0);

  /**
   * Copy a field by creating new data arrays (i.e., duplicate storage).
   */
  virtual void DeepCopy(svtkFieldData* da);

  /**
   * Copy a field by reference counting the data arrays.
   */
  virtual void ShallowCopy(svtkFieldData* da);

  /**
   * Squeezes each data array in the field (Squeeze() reclaims unused memory.)
   */
  void Squeeze();

  /**
   * Resets each data array in the field (Reset() does not release memory but
   * it makes the arrays look like they are empty.)
   */
  void Reset();

  /**
   * Return the memory in kibibytes (1024 bytes) consumed by this field data. Used to
   * support streaming and reading/writing data. The value returned is
   * guaranteed to be greater than or equal to the memory required to
   * actually represent the data represented by this object.
   */
  virtual unsigned long GetActualMemorySize();

  /**
   * Check object's components for modified times.
   */
  svtkMTimeType GetMTime() override;

  /**
   * Get a field from a list of ids. Supplied field f should have same
   * types and number of data arrays as this one (i.e., like
   * CopyStructure() creates).  This method should not be used if the
   * instance is from a subclass of svtkFieldData (svtkPointData or
   * svtkCellData).  This is because in those cases, the attribute data
   * is stored with the other fields and will cause the method to
   * behave in an unexpected way.
   */
  void GetField(svtkIdList* ptId, svtkFieldData* f);

  /**
   * Return the array containing the ith component of the field. The
   * return value is an integer number n 0<=n<this->NumberOfArrays. Also,
   * an integer value is returned indicating the component in the array
   * is returned. Method returns -1 if specified component is not
   * in the field.
   */
  int GetArrayContainingComponent(int i, int& arrayComp);

  /**
   * Get the number of components in the field. This is determined by adding
   * up the components in each non-nullptr array.
   * This method should not be used if the instance is from a
   * subclass of svtkFieldData (svtkPointData or svtkCellData).
   * This is because in those cases, the attribute data is
   * stored with the other fields and will cause the method
   * to behave in an unexpected way.
   */
  int GetNumberOfComponents();

  /**
   * Get the number of tuples in the field. Note: some fields have arrays with
   * different numbers of tuples; this method returns the number of tuples in
   * the first array. Mixed-length arrays may have to be treated specially.
   * This method should not be used if the instance is from a
   * subclass of svtkFieldData (svtkPointData or svtkCellData).
   * This is because in those cases, the attribute data is
   * stored with the other fields and will cause the method
   * to behave in an unexpected way.
   */
  svtkIdType GetNumberOfTuples();

  /**
   * Set the number of tuples for each data array in the field.
   * This method should not be used if the instance is from a
   * subclass of svtkFieldData (svtkPointData or svtkCellData).
   * This is because in those cases, the attribute data is
   * stored with the other fields and will cause the method
   * to behave in an unexpected way.
   */
  void SetNumberOfTuples(const svtkIdType number);

  /**
   * Set the jth tuple in source field data at the ith location.
   * Set operations mean that no range checking is performed, so
   * they're faster.
   */
  void SetTuple(const svtkIdType i, const svtkIdType j, svtkFieldData* source);

  /**
   * Insert the jth tuple in source field data at the ith location.
   * Range checking is performed and memory allocates as necessary.
   */
  void InsertTuple(const svtkIdType i, const svtkIdType j, svtkFieldData* source);

  /**
   * Insert the jth tuple in source field data at the end of the
   * tuple matrix. Range checking is performed and memory is allocated
   * as necessary.
   */
  svtkIdType InsertNextTuple(const svtkIdType j, svtkFieldData* source);

protected:
  svtkFieldData();
  ~svtkFieldData() override;

  int NumberOfArrays;
  int NumberOfActiveArrays;
  svtkAbstractArray** Data;

  /**
   * Set an array to define the field.
   */
  void SetArray(int i, svtkAbstractArray* array);

  /**
   * Release all data but do not delete object.
   */
  virtual void InitializeFields();

  struct CopyFieldFlag
  {
    char* ArrayName;
    int IsCopied;
  };

  CopyFieldFlag* CopyFieldFlags; // the names of fields not to be copied
  int NumberOfFieldFlags;        // the number of fields not to be copied
  void CopyFieldOnOff(const char* name, int onOff);
  void ClearFieldFlags();
  int FindFlag(const char* field);
  int GetFlag(const char* field);
  void CopyFlags(const svtkFieldData* source);
  int DoCopyAllOn;
  int DoCopyAllOff;

private:
  svtkFieldData(const svtkFieldData&) = delete;
  void operator=(const svtkFieldData&) = delete;

public:
  class SVTKCOMMONDATAMODEL_EXPORT BasicIterator
  {
  public:
    BasicIterator();
    BasicIterator(const BasicIterator& source);
    BasicIterator(const int* list, unsigned int listSize);
    BasicIterator& operator=(const BasicIterator& source);
    virtual ~BasicIterator();
    void PrintSelf(ostream& os, svtkIndent indent);

    int GetListSize() const { return this->ListSize; }
    int GetCurrentIndex() { return this->List[this->Position]; }
    int BeginIndex()
    {
      this->Position = -1;
      return this->NextIndex();
    }
    int End() const { return (this->Position >= this->ListSize); }
    int NextIndex()
    {
      this->Position++;
      return (this->End() ? -1 : this->List[this->Position]);
    }

  protected:
    int* List;
    int ListSize;
    int Position;
  };

  class SVTKCOMMONDATAMODEL_EXPORT Iterator : public BasicIterator
  {
  public:
    Iterator(const Iterator& source);
    Iterator& operator=(const Iterator& source);
    ~Iterator() override;
    Iterator(svtkFieldData* dsa, const int* list = nullptr, unsigned int listSize = 0);

    svtkDataArray* Begin()
    {
      this->Position = -1;
      return this->Next();
    }

    svtkDataArray* Next()
    {
      this->Position++;
      if (this->End())
      {
        return nullptr;
      }

      // svtkFieldData::GetArray() can return null, which implies that
      // a the array at the given index in not a svtkDataArray subclass.
      // This iterator skips such arrays.
      svtkDataArray* cur = Fields->GetArray(this->List[this->Position]);
      return (cur ? cur : this->Next());
    }

    void DetachFieldData();

  protected:
    svtkFieldData* Fields;
    int Detached;
  };
};

#endif
