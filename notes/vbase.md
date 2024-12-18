VBASE Unifying Online Vector Similarity Search and Relational Queries via  Relaxed Monotonicity

与传统的标量索引不同，高维向量索引是为近似 TopK 设计的，不遵循单调性。图 1 显示了两个流行的向量索引 FAISS IVVFlat [5] 和 HNSW [89] 的 TopK 遍历模式。如图所示，向量索引遍历不符合单调性，随着遍历的进行，与目标向量的距离会不可预测地振荡。这些向量索引中缺乏单调性，使关系数据库无法直接使用它们来加速查询 。


