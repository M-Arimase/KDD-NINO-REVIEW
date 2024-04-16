#include "NinoSketch.hpp"
#include "datatypes.hpp"
#include <algorithm>
#include <math.h>

ElasticSketch::ElasticSketch(int l2_depth, int l2_width, int lgn) {
  this->l2_depth = l2_depth;
  this->l2_width = l2_width;
  this->lgn = lgn;

  this->l2_fprint = new std::bitset<ES_FP>[l2_depth * l2_width];
  this->l2_counter = new uint32_t[l2_depth * l2_width];
  for (int i = 0; i < l2_depth * l2_width; i++) {
    this->l2_fprint[i] = NULL;
    this->l2_counter[i] = 0;
  }

  this->l2_vote = new uint32_t[l2_width];
  for (int i = 0; i < l2_width; i++) {
    this->l2_vote[i] = 0;
  }

  char name2[] = "ElasticSketch-L2";
  unsigned long seed = AwareHash((unsigned char *)name2, strlen(name2),
                                 13091204281, 228204732751, 6620830889);
  this->l2_hardner = GenHashSeed(seed++);
}

std::bitset<ES_FP> *ElasticSketch::Key2FP(const unsigned char *key,
                                          int key_len) {
  unsigned long hashed = MurmurHash64A(key, key_len, 10010);
  return new std::bitset<ES_FP>(hashed);
}

void ElasticSketch::Update(unsigned char *key, val_tp value) {
  unsigned long col = 0;
  int key_len = this->lgn / 8;
  int l2_index;
  std::bitset<ES_FP> *l2_fp = this->Key2FP(key, key_len);

  unsigned long hashes;
  hashes = MurmurHash64A(key, key_len, this->l2_hardner);
  col = hashes % this->l2_width;

  for (int i = 0; i < this->l2_depth; i++) {
    l2_index = i * this->l2_width + col;
    if (l2_fprint[l2_index] != NULL && l2_fprint[l2_index] == *l2_fp) {
      l2_counter[l2_index] += 1;
      return;
    }
  }

  int l2_smallest_count = 8888888;
  int l2_smallest_idx = -1;

  for (int i = 0; i < this->l2_depth; i++) {
    l2_index = i * this->l2_width + col;

    if (l2_fprint[l2_index] == NULL) {
      l2_fprint[l2_index] = *l2_fp;
      l2_counter[l2_index] = 1;
      return;
    } else if (l2_counter[l2_index] < l2_smallest_count) {
      l2_smallest_count = l2_counter[l2_index];
      l2_smallest_idx = l2_index;
    }
  }

  if (l2_smallest_idx != -1) {
    l2_vote[col] += 1;
    if (l2_vote[col] > ES_LAMBDA * l2_smallest_count) {
      l2_fprint[l2_smallest_idx] = *l2_fp;
      l2_counter[l2_smallest_idx] = 1;
      l2_vote[col] = 0;
    }
  }
  return;
}

void ElasticSketch::Query(val_tp thresh,
                          std::vector<std::pair<l2_fp_tp, val_tp>> &results) {
  for (int i = 0; i < this->l2_width * this->l2_depth; i++) {
    val_tp val = this->l2_counter[i];
    if (val > thresh) {
      l2_fp_tp fp = this->l2_fprint[i];
      std::pair<l2_fp_tp, val_tp> node;
      node.first = fp;
      node.second = val;
      results.push_back(node);
    }
  }
}

val_tp ElasticSketch::QueryL2(unsigned char *key) {
  unsigned long col = 0;
  int key_len = this->lgn / 8;
  int l2_index;
  std::bitset<ES_FP> *l2_fp = this->Key2FP(key, key_len);

  unsigned long hashed = MurmurHash64A(key, key_len, this->l2_hardner);
  col = hashed % this->l2_width;
  for (int i = 0; i < this->l2_depth; i++) {
    l2_index = i * this->l2_width + col;
    if (l2_fprint[l2_index] != NULL && l2_fprint[l2_index] == *l2_fp) {
      return l2_counter[l2_index];
    }
  }
  return 0;
}

MESketch::MESketch(int l1_depth, int l1_width, int l2_depth, int l2_width,
                   int lgn, bool use_flag /*= true*/) {
  this->l1_depth = l1_depth;
  this->l2_depth = l2_depth;
  this->l1_width = l1_width;
  this->l2_width = l2_width;
  this->lgn = lgn;
  this->use_flag = use_flag;
  this->sum = 0;
  l1_hash = new unsigned long[l1_depth];
  l1_scale = new unsigned long[l1_depth];
  l1_hardner = new unsigned long[l1_depth];
  l2_hash = new unsigned long[l2_depth];
  l2_scale = new unsigned long[l2_depth];
  l2_hardner = new unsigned long[l2_depth];
  this->l1_table = new Bucket *[l1_depth * l1_width];
  for (int i = 0; i < l1_depth * l1_width; i++) {
    this->l1_table[i] = new Bucket(use_flag);
  }
  this->l2_table = new L2Cell *[l2_depth * l2_width];
  for (int i = 0; i < l2_depth * l2_width; i++) {
    this->l2_table[i] = new L2Cell(use_flag);
  }

  char name[] = "MESketch-L1";
  unsigned long seed = AwareHash((unsigned char *)name, strlen(name),
                                 13091204281, 228204732751, 6620830889);
  for (int i = 0; i < l1_depth; i++) {
    this->l1_hash[i] = GenHashSeed(seed++);
  }
  for (int i = 0; i < l1_depth; i++) {
    this->l1_scale[i] = GenHashSeed(seed++);
  }
  for (int i = 0; i < l1_depth; i++) {
    this->l1_hardner[i] = GenHashSeed(seed++);
  }

  char name2[] = "MESketch-L2";
  seed = AwareHash((unsigned char *)name2, strlen(name2), 13091204281,
                   228204732751, 6620830889);
  for (int i = 0; i < l2_depth; i++) {
    this->l2_hash[i] = GenHashSeed(seed++);
  }
  for (int i = 0; i < l2_depth; i++) {
    this->l2_scale[i] = GenHashSeed(seed++);
  }
  for (int i = 0; i < l2_depth; i++) {
    this->l2_hardner[i] = GenHashSeed(seed++);
  }
  quick_start = true;
}

MESketch::~MESketch() {
  for (int i = 0; i < l1_depth * l1_width; i++) {
    delete l1_table[i];
  }
  for (int i = 0; i < l2_depth * l2_width; i++) {
    delete l2_table[i];
  }

  delete[] l1_hardner;
  delete[] l2_hardner;
  delete[] l1_hash;
  delete[] l2_hash;
  delete[] l1_scale;
  delete[] l2_scale;
}

fp_pair MESketch::Key2FP(const unsigned char *key, int key_len) {
  unsigned long hashed = MurmurHash64A(key, key_len, 10010);
  fp_pair node;
  node.first = new std::bitset<L1_FP>(hashed);
  node.second = new std::bitset<L2_FP>(hashed);
  return node;
}

bool MESketch::MoveAndIncrement(Bucket *src, L2Cell *dst, l2_fp_tp l2_fp) {
  int idx = dst->AddNewElement(l2_fp, src->GetCounter());
  dst->Increment(idx);
  return true;
}

void MESketch::Update(unsigned char *key, val_tp value) {
  unsigned long col = 0;
  int key_len = this->lgn / 8;
  int l1_index;
  int l2_index;
  int bucket_idx;
  double r;
  L2Cell *cell;
  Bucket *bucket;
  fp_pair pair = this->Key2FP(key, key_len);
  std::bitset<L1_FP> *l1_fp = pair.first;
  std::bitset<L2_FP> *l2_fp = pair.second;

  unsigned long hashes[this->l2_depth];
  for (int i = 0; i < this->l2_depth; i++) {
    hashes[i] = MurmurHash64A(key, key_len, this->l2_hardner[i]);
  }

  if (!quick_start) {
    for (int i = 0; i < this->l2_depth; i++) {
      col = hashes[i] % this->l2_width;
      l2_index = i * this->l2_width + col;
      cell = this->l2_table[l2_index];
      bucket_idx = cell->InCell(*l2_fp);
      if (bucket_idx >= 0) {
        if (cell->GetFlag(bucket_idx) == 0) {
          cell->Increment(bucket_idx);
        }
        return;
      }
    }
  }

  int l1_smallest_count = 8888888;
  int l1_smallest_idx = -1;

  for (int i = 0; i < this->l1_depth; i++) {
    col = hashes[i] % this->l1_width;
    l1_index = i * this->l1_width + col;
    bucket = this->l1_table[l1_index];

    if (bucket->IsEmptyBucket()) {
      bucket->SetFP(*l1_fp);
      bucket->SetCounterSize(L1_COUNTER, 1);
      return;
    } else if (bucket->FPEqual(*l1_fp)) {
      if (bucket->GetFlag() == 0) {
        if (bucket->IsFull()) {
          quick_start = false;
          uint64_t l2_smallest_count = 88888888888;
          int l2_smallest_idx = -1;
          for (int j = 0; j < this->l2_depth; j++) {
            col = hashes[j] % this->l2_width;
            l2_index = j * this->l2_width + col;
            cell = this->l2_table[l2_index];
            if (cell->HasEmptyBucket()) {
              MoveAndIncrement(bucket, cell, *l2_fp);
              bucket->ClearBucket();
              return;
            }
            int smallest_idx = cell->GetBucketNum() - 1;
            uint64_t count = cell->GetCounter(smallest_idx).to_ulong();
            if (count < l2_smallest_count) {
              l2_smallest_count = count;
              l2_smallest_idx = l2_index;
            }
          }
          if (l2_smallest_idx != -1) {
            cell = this->l2_table[l2_smallest_idx];
            int smallest_idx = cell->GetBucketNum() - 1;
            r = (double)rand() / (RAND_MAX);
            if (r < 1. / ((double)cell->GetCounter(smallest_idx).to_ulong())) {
              {
                cell->ReplaceElement(smallest_idx, *l2_fp,
                                     bucket->GetCounter());
                cell->Increment(smallest_idx);
                bucket->ClearBucket();
                return;
              }
            }
          }
        } else {
          bucket->Increment();
          return;
        }
      }
    }

    int count = bucket->GetCounter().to_ulong();

    if (count < l1_smallest_count) {
      l1_smallest_count = count;
      l1_smallest_idx = l1_index;
    }
  }
  if (l1_smallest_idx >= 0) {
    bucket = this->l1_table[l1_smallest_idx];
    r = (double)rand() / RAND_MAX;
    if (r < 1. / (double)bucket->GetCounter().to_ulong()) {
      {
        bucket->ClearBucket();
        bucket->SetFP(*l1_fp);
        bucket->Increment();
      }
    }
  }
}

void MESketch::NewWindow() {
  for (int i = 0; i < this->l1_depth * this->l1_width; i++) {
    l1_table[i]->ResetFlag();
  }

  for (int i = 0; i < this->l2_depth * this->l2_width; i++) {
    l2_table[i]->ResetFlags();
  }
}

void MESketch::Query(val_tp thresh,
                     std::vector<std::pair<l2_fp_tp, val_tp>> &results) {
  int bucket_num;
  for (int i = 0; i < this->l2_width * this->l2_depth; i++) {
    bucket_num = this->l2_table[i]->GetBucketNum();
    for (int j = 0; j < bucket_num; j++) {
      val_tp val = this->l2_table[i]->GetCounter(j).to_ulong();
      if (val > thresh) {
        l2_fp_tp fp = this->l2_table[i]->GetFP(j);
        std::pair<l2_fp_tp, val_tp> node;
        node.first = fp;
        node.second = val;
        results.push_back(node);
      }
    }
  }
}

val_tp MESketch::QueryL1(unsigned char *key) {
  unsigned long col = 0;
  int key_len = this->lgn / 8;
  int l1_index;
  Bucket *bucket;
  fp_pair pair = this->Key2FP(key, key_len);
  std::bitset<L1_FP> *l1_fp = pair.first;
  val_tp min_val = 8888888;
  val_tp bucket_val;
  for (int i = 0; i < this->l1_depth; i++) {
    unsigned long hashed = MurmurHash64A(key, key_len, this->l1_hardner[i]);
    col = hashed % this->l1_width;
    l1_index = i * this->l1_width + col;
    bucket = this->l1_table[l1_index];
    bucket_val = bucket->GetCounter().to_ulong();
    if (bucket->FPEqual(*l1_fp)) {
      return bucket_val;
    } else if (bucket_val < min_val) {
      min_val = bucket_val;
    }
  }
  return min_val;
}

val_tp MESketch::QueryL2(unsigned char *key) {
  unsigned long col = 0;
  int key_len = this->lgn / 8;
  int l2_index;
  L2Cell *cell;
  int bucket_idx;
  fp_pair pair = this->Key2FP(key, key_len);
  std::bitset<L2_FP> *l2_fp = pair.second;
  val_tp min_val = 8888888888888888;
  for (int i = 0; i < this->l2_depth; i++) {
    unsigned long hashed = MurmurHash64A(key, key_len, this->l2_hardner[i]);
    col = hashed % this->l2_width;
    l2_index = i * this->l2_width + col;
    cell = this->l2_table[l2_index];
    bucket_idx = cell->InCell(*l2_fp);
    if (bucket_idx >= 0) {
      return cell->GetCounter(bucket_idx).to_ulong();
    }
  }
  return this->QueryL1(key);
}

void MESketch::PrintBucketStatus() {
  int bucket_num;
  int total_bucket_num = 0;
  int empty_bucket_num = 0;
  for (int i = 0; i < this->l2_width * this->l2_depth; i++) {
    bucket_num = this->l2_table[i]->GetBucketNum();
    total_bucket_num += bucket_num;
    for (int j = 0; j < bucket_num; j++) {
      if (this->l2_table[i]->GetFP(j) == NULL) {
        empty_bucket_num += 1;
      }
    }
  }

  std::cout << empty_bucket_num << "/" << total_bucket_num << std::endl;
}
