// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdIDataPointDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "include/DataPoint.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace project {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *project_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("project", 0 /*version*/, "DataPoint.h", 12,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &project_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *project_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_projectcLcLDataPoint(void *p = nullptr);
   static void *newArray_projectcLcLDataPoint(Long_t size, void *p);
   static void delete_projectcLcLDataPoint(void *p);
   static void deleteArray_projectcLcLDataPoint(void *p);
   static void destruct_projectcLcLDataPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::project::DataPoint*)
   {
      ::project::DataPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::project::DataPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("project::DataPoint", ::project::DataPoint::Class_Version(), "DataPoint.h", 13,
                  typeid(::project::DataPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::project::DataPoint::Dictionary, isa_proxy, 4,
                  sizeof(::project::DataPoint) );
      instance.SetNew(&new_projectcLcLDataPoint);
      instance.SetNewArray(&newArray_projectcLcLDataPoint);
      instance.SetDelete(&delete_projectcLcLDataPoint);
      instance.SetDeleteArray(&deleteArray_projectcLcLDataPoint);
      instance.SetDestructor(&destruct_projectcLcLDataPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::project::DataPoint*)
   {
      return GenerateInitInstanceLocal(static_cast<::project::DataPoint*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::project::DataPoint*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace project {
//______________________________________________________________________________
atomic_TClass_ptr DataPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DataPoint::Class_Name()
{
   return "project::DataPoint";
}

//______________________________________________________________________________
const char *DataPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::project::DataPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DataPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::project::DataPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DataPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::project::DataPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DataPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::project::DataPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace project
namespace project {
//______________________________________________________________________________
void DataPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class project::DataPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(project::DataPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(project::DataPoint::Class(),this);
   }
}

} // namespace project
namespace ROOT {
   // Wrappers around operator new
   static void *new_projectcLcLDataPoint(void *p) {
      return  p ? new(p) ::project::DataPoint : new ::project::DataPoint;
   }
   static void *newArray_projectcLcLDataPoint(Long_t nElements, void *p) {
      return p ? new(p) ::project::DataPoint[nElements] : new ::project::DataPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_projectcLcLDataPoint(void *p) {
      delete (static_cast<::project::DataPoint*>(p));
   }
   static void deleteArray_projectcLcLDataPoint(void *p) {
      delete [] (static_cast<::project::DataPoint*>(p));
   }
   static void destruct_projectcLcLDataPoint(void *p) {
      typedef ::project::DataPoint current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::project::DataPoint

namespace {
  void TriggerDictionaryInitialization_DataPointDict_Impl() {
    static const char* headers[] = {
"include/DataPoint.h",
nullptr
    };
    static const char* includePaths[] = {
"/opt/apps/root/include",
"/opt/apps/root/include/",
"/home/hannap/mnxb11/MNXB11-project/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "DataPointDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace project{class __attribute__((annotate("$clingAutoload$include/DataPoint.h")))  DataPoint;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DataPointDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "include/DataPoint.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"project::DataPoint", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DataPointDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DataPointDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DataPointDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DataPointDict() {
  TriggerDictionaryInitialization_DataPointDict_Impl();
}
