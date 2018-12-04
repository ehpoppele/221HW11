#pragma once

#include "climb_chromosome.hh"

class HgrexChromosome : public ClimbChromosome
{
	public:
        //Explicitly define constructor
        HgrexChromosome(const Cities* cities)
        : ClimbChromosome(cities)
        {}

        // Explicitly define destructor
        virtual ~HgrexChromosome() = default;
        
        //In this case we override recombine to improve the algorithm. This is an overloaded method, not an overridden one.
        virtual std::pair<Chromosome*, Chromosome*> recombine(const HgrexChromosome* other);
        
        //We also add a new helper method for the new recombine. This more or less replaces "create_crossover_child",
        //but works slightly differently so I'm just making this new method instead of overriding the old one.
        HgrexChromosome* hgrex_crossover_child(const HgrexChromosome* p1,
                            const HgrexChromosome* p2) const;
                            
        //Helper function for new recombine; returns an appropriate random integer not appearing in the given range
        unsigned random_unused(unsigned begin, unsigned end) const;

        //Helper function for new recombine; returns the index that element x appears at. Perhaps I could have used an stl function here.
        unsigned find_index(unsigned x) const;
        
        virtual HgrexChromosome* clone() const override
        {
            return new HgrexChromosome(*this);
        }
        
};