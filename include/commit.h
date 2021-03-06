/*
 * Copyright 2011, Tim Branyen @tbranyen <tim@tabdeveloper.com>
 * @author Michael Robinson @codeofinterest <mike@pagesofinterest.net>
 *
 * Dual licensed under the MIT and GPL licenses.
 */

#include <v8.h>
#include <node.h>
#include <vector>

#include "git2.h"

#include "reference.h"
#include "repo.h"
#include "oid.h"
#include "tree.h"

using namespace node;
using namespace v8;

/**
 * Class wrapper for libgit2 git_commit
 */
class GitCommit : public ObjectWrap {
  public:
    /**
     * v8::FunctionTemplate used to create Node.js constructor
     */
    static Persistent<Function> constructor_template;

    /**
     * Used to intialize the EventEmitter from Node.js
     *
     * @param target v8::Object the Node.js module object
     */
    static void Initialize (Handle<v8::Object> target);

    git_commit* GetValue();
    void SetValue(git_commit* commit);
    void SetOid(git_oid* oid);

  protected:
    GitCommit() {}
    ~GitCommit() {}

    static Handle<Value> New(const Arguments& args);
    static Handle<Value> Free(const Arguments& args);

    static Handle<Value> Lookup(const Arguments& args);
    static void LookupWork(uv_work_t *req);
    static void LookupAfterWork(uv_work_t *req);

    static Handle<Value> Oid(const Arguments& args);

    static Handle<Value> Message(const Arguments& args);
    static void MessageWork(uv_work_t* req);
    static void MessageAfterWork(uv_work_t* req);

    static Handle<Value> Time(const Arguments& args);
    static void TimeWork(uv_work_t* req);
    static void TimeAfterWork(uv_work_t* req);

    static Handle<Value> Offset(const Arguments& args);
    static void OffsetWork(uv_work_t* req);
    static void OffsetAfterWork(uv_work_t* req);

    static Handle<Value> Author(const Arguments& args);
    static void AuthorWork(uv_work_t* req);
    static void AuthorAfterWork(uv_work_t* req);

    static Handle<Value> Committer(const Arguments& args);
    static void CommitterWork(uv_work_t* req);
    static void CommitterAfterWork(uv_work_t* req);

    static Handle<Value> Tree(const Arguments& args);
    static void TreeWork(uv_work_t* req);
    static void TreeAfterWork(uv_work_t* req);

    static Handle<Value> Parents(const Arguments& args);
    static void ParentsWork(uv_work_t* req);
    static void ParentsAfterWork(uv_work_t* req);

  private:
    git_commit* commit;
    git_oid* oid;

    struct LookupBaton {
      uv_work_t request;
      const git_error* error;

      git_repository* repo;
      git_oid rawOid;
      std::string sha;
      git_commit* rawCommit;

      Persistent<Function> callback;
    };

    struct MessageBaton {
      uv_work_t request;

      git_commit* rawCommit;
      std::string message;

      Persistent<Function> callback;
    };

    struct TimeBaton {
      uv_work_t request;

      git_commit* rawCommit;
      git_time_t time;

      Persistent<Function> callback;
    };

    struct SignatureBaton {
      uv_work_t request;

      git_commit* rawCommit;
      const git_signature* rawSignature;

      Persistent<Function> callback;
    };

    struct OffsetBaton {
      uv_work_t request;

      git_commit* rawCommit;
      int offset;

      Persistent<Function> callback;
    };


    struct TreeBaton {
      uv_work_t request;
      const git_error* error;

      git_commit* rawCommit;
      git_tree* rawTree;

      Persistent<Function> callback;
    };

    struct Parent {
      const git_oid* rawOid;
      git_commit* rawCommit;
    };

    struct ParentsBaton {
      uv_work_t request;
      const git_error* error;

      int index;
      git_commit* rawCommit;
      std::vector<Parent* > parents;

      Persistent<Function> callback;
    };
};
