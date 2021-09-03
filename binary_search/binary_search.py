#!/usr/bin/env python
# coding: utf-8

# ## Imports

# In[ ]:


import struct


# ## Opening file for first time to set variables

# In[ ]:


registroCEP = struct.Struct("72s72s72s72s2s8s2s")

with open("cep_ordenado.dat") as file:

    line_size = registroCEP.size
    line_content_init = []
    line_content_final = []
    cep_init = []
    cep_final = []
    
    #CEP in initial position
    pos_init = file.seek(0, 0)
    line_content_init = file.readline()
    cep_init = line_content_init[-10:-2]

    
    #CEP in final position
    pos_final = file.seek(0, 2)
    file.seek(pos_final - line_size, 0)
    line_content_final = file.readline()
    cep_final = line_content_final[-10:-2]
    
    
    print(pos_init, pos_final)


# ## Binary Search

# In[ ]:


def binary_search(query, pos_i, pos_f):
    registroCEP = struct.Struct("72s72s72s72s2s8s2s")

    with open("cep_ordenado.dat") as file:

        #CEP in initial position
        pos_init = file.seek(pos_i, 0)
        line_content_init = file.readline()
        cep_init = line_content_init[-10:-2]


        #CEP in final position
        pos_final = file.seek(pos_f, 0)
        file.seek(pos_final, 0)
        line_content_final = file.readline()
        cep_final = line_content_final[-10:-2]
        
        
        pos_mid = round((pos_init + pos_final)/2)
        file.seek(pos_mid, 0)
        line_content_mid = file.readline()
        cep_mid = line_content_mid[-10:-2]
        
        print(pos_init, pos_final)

        if query == cep_mid:
            print("O CEP esta no Byte "+ str(pos_mid))
            return pos_mid

        elif query < cep_mid:
            return binary_search(query, pos_init, pos_mid)
        elif query > cep_mid:
            return binary_search(query, pos_mid, pos_final)


# In[ ]:


search = binary_search('03978630', pos_init, pos_final)

with open("cep_ordenado.dat") as file:

    pos_search = file.seek(search, 0)
    line_content_search = file.readline()
    cep_search = line_content_search[-12:-2]

    print(cep_search)


# In[ ]:




